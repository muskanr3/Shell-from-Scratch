#include "headers.h"
#include "cmd.h"
#include "jobs.h"
#include "fg.h"
#include "sig.h"
#include <signal.h>

int main()
{
    signal(SIGINT, sigC);

    struct sigaction sigact;
    sigact.sa_handler = sigZ;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGTSTP, &sigact, NULL);

    FILE *fd = fopen("history.txt", "r");

    // building the prompt path
    char host[10000];
    char name[10000];
    char cwd[10000];
    char path[10000];
    char prev[10000];

    char *cmd;
    cmd = (char *)malloc(sizeof(char) * 10000);
    char res[10000];
    char lastdir[10000];

    strcpy(name, getlogin());
    gethostname(host, 10000);
    getcwd(cwd, 10000);
    strcat(name, "@");
    strcat(host, ":~");
    strcat(name, host);
    strcpy(path, name);

    // host that stays const till :~
    char root[10000];
    int cdcount = 1;
    char *buf;
    size_t bufsize = 50;
    buf = (char *)malloc(bufsize * sizeof(char));
    char cdpath[10000];
    getcwd(cdpath, 10000);

    char hispath[10000];
    getcwd(hispath, 10000);
    strcat(hispath, "/history.txt");

    char *inputFile;
    inputFile = (char *)malloc(sizeof(char) * 10000);
    char *command;
    command = (char *)malloc(sizeof(char) * 10000);
    char *outputFile;
    outputFile = (char *)malloc(sizeof(char) * 10000);
    memset(inputFile, '\0', 10000);
    memset(outputFile, '\0', 10000);
    memset(command, '\0', 10000);

    int outflag;
    int inflag;
    int apflag;

    while (1)
    {
        strcpy(root, name); // resetting the root to original root

        char cmd2[10000];
        printf("\033[1;32m<%s> ", path);

        printf("\033[0;37m");
        char *s;
        s = (char *)malloc(sizeof(char) * 10000);
        s = fgets(cmd, 10000, stdin);

        if (s == NULL)
        {
            // ctrl D
            // kill all processes and exit
            struct node *ptr;
            ptr = head;
            if (ptr == NULL)
            {
                printf("No background processes currently.\n");
            }
            else
            {
                while (ptr != NULL)
                {
                    kill(ptr->pid, SIGKILL);
                    delete_node(ptr->pid);
                    ptr = ptr->next;
                }
            }
            printf("\n");
            exit(0);
        }

        if (strcmp(cmd, "\n") == 0)
            continue;
        // stripping the newline
        int len = strlen(cmd);
        if (len > 0 && cmd[len - 1] == '\n')
            cmd[len - 1] = '\0';

        // checking for redirection
        checkSymbol(cmd, &apflag, &inflag, &outflag, command, inputFile, outputFile);

        check(&apflag, &inflag, &outflag, inputFile, outputFile);
        strcpy(cmd2, cmd);

        addCommand(cmd, hispath); // adding the command to history.txt

        // checking for piping and executing if its required
        executePipe(cmd, &apflag, &inflag, &outflag, inputFile, outputFile);

        char *token = cmd;

        while (token != NULL)
        {
            token = strtok_r(cmd, "&;", &cmd);
            if (token == NULL)
                break;

            char str[10000];
            char *cmdtok;
            cmdtok = token;

            char *new;
            new = (char *)malloc(sizeof(char) * 10000);
            new = strtok_r(cmdtok, " \t\n", &cmdtok);

            if (strcmp(cmd2, "\n") == 0)
            {
                continue;
            }
            if (strcmp(new, "cd") == 0)
            {
                // if there is a word after cd

                if (strcmp("\0", cmdtok))
                {
                    if (strcmp("..", cmdtok) == 0)
                    {
                        cdcount = 1;
                        getcwd(lastdir, 10000);

                        cdDotDot(path);
                        char *ptr = strrchr(path, '/');
                        if (ptr == NULL)
                        {
                            // if we are already in ~ we print the absolute path
                            getcwd(res, 10000);
                            strcat(root, res);
                            strcpy(path, root);
                            continue;
                        }
                        else
                        {
                            *ptr = '\0';
                        }
                    }

                    else if (strcmp(".", cmdtok) == 0)
                    {
                        cdcount = 1;
                        getcwd(lastdir, 10000);
                        cdDot(path);
                        continue;
                    }

                    else if (strcmp("-", cmdtok) == 0)
                    {
                        getcwd(prev, 10000);
                        chdir(lastdir);
                        if (cdcount == 2)
                        {
                            char *ptr1 = strrchr(lastdir, '/');
                            char ptr2[10000];
                            strcpy(ptr2, strrchr(prev, '/'));
                            strcat(ptr2, ptr1);
                            strcat(root, ptr2);
                            strcpy(path, root);
                            strcpy(lastdir, prev);
                            cdcount = 1;
                        }

                        else
                        {
                            char *ptr1 = strrchr(lastdir, '/');
                            strcat(root, ptr1);
                            strcpy(path, root);
                            strcpy(lastdir, prev);
                            cdcount++;
                        }

                        continue;
                    }

                    else if (strcmp("~", cmdtok) == 0)
                    {
                        cdcount = 1;
                        getcwd(lastdir, 10000);
                        strcpy(path, name);
                        cdDot(path);
                    }

                    else
                    {
                        cdcount = 1;
                        getcwd(lastdir, 10000);
                        if (open(cmdtok, O_RDONLY, 0) != -1)
                        {
                            strcat(path, "/");
                            strcat(path, cmdtok);
                            cdFile(cmdtok);
                        }

                        else
                        {
                            perror("\033[1;31mError opening file\033[0m");
                            printf("\x1B[37m");
                        }
                    }
                }

                else
                {
                    // if there is no word after cd
                    getcwd(lastdir, 10000);
                    cdcount = 1;
                    strcpy(name, getlogin());
                    gethostname(host, 10000);
                    getcwd(cwd, 10000);
                    strcat(name, "@");
                    strcat(host, ":~");
                    strcat(name, host);
                    strcpy(path, name);

                    chdir(cdpath);
                }

                continue;
            }

            else if (strcmp(new, "pwd") == 0)
            {
                pwd(&apflag, &inflag, &outflag, inputFile, outputFile);
            }

            else if (strcmp("jobs", new) == 0)
            {
                jobs(cmd2, &apflag, &inflag, &outflag, inputFile, outputFile);
            }

            else if (strcmp("sig", new) == 0)
            {
                sig(cmd2);
            }

            else if (strcmp(new, "discover") == 0)
            {
                discover(command, cdpath, &apflag, &inflag, &outflag, inputFile, outputFile);
            }
            else if (strcmp(new, "pinfo") == 0)
            {
                int pid = getpid();
                pinfo(pid, command, cdpath, &apflag, &inflag, &outflag, inputFile, outputFile);
            }

            else if (strcmp(new, "bg") == 0)
            {
                backgrd(cmd2);
            }

            else if (strcmp(new, "exit") == 0)
            {
                exit(0);
            }

            else if (strcmp(new, "fg") == 0)
            {
                backToForeground(cmd2);
            }
            else if (strcmp(new, "ls") == 0)
            {
                ls(cmd2, cwd, &apflag, &inflag, &outflag, inputFile, outputFile);
            }

            else if (strcmp(new, "echo") == 0)
            {
                echo(str, command, &apflag, &inflag, &outflag, inputFile, outputFile);
            }

            else if (strcmp(new, "history") == 0)
            {
                printHis(hispath, &apflag, &inflag, &outflag, inputFile, outputFile);
            }

            else
            {
                // running foreground and background processes
                fb(cmd2, &apflag, &inflag, &outflag, inputFile, outputFile);
                break;
            }
            getcwd(lastdir, 10000);
        }

        // resetting variable for next iteration
        apflag = 0;
        inflag = 0;
        outflag = 0;
        memset(inputFile, '\0', 10000);
        memset(outputFile, '\0', 10000);
        memset(command, '\0', 10000);
    }
    fclose(fd);
    return 0;
}