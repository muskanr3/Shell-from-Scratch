#include "headers.h"
#include <time.h>
#include "cmd.h"
#include "jobs.h"
#include "sig.h"

// struct for storing the ongoing background processes
struct bground
{
    char *comm;
    int pid;
};

int ord = 1;
int childPid = 0;
int bind = 0; // index of the struct

typedef struct bground bground;

bground back[10000];

char *args[10000]; // array of commands for execvp
void check(int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
{
    int stat;

    while (1)
    {
        // checks if there exists a child in the struct that is still running in the background
        int checkpid = waitpid(-1, &stat, WNOHANG | WUNTRACED);

        for (int i = 0; i < bind; i++)
        {
            if (back[i].pid == checkpid)
            {
                if (WIFEXITED(stat))
                {
                    printf("%s with pid %d exited normally.\n", back[i].comm, back[i].pid);
                    delete_node(back[i].pid); // delete node if exited normally
                }
                else
                {
                    printf("%s with pid %d exited abnormally.\n", back[i].comm, back[i].pid);
                    // change status to stopped if exited abnormally
                    changeStatus(back[i].pid);
                }
            }
        }

        if (checkpid <= 0) // there is no ongoing child process
            break;
    }
}

void makeCommandArray(char *cmd)
{
    // create a null terminated array of commands to pass to execvp
    char *token;
    char *cmdcopy = cmd;
    int ind = 0;

    while ((token = strtok_r(cmdcopy, " ", &cmdcopy)))
    {
        if (strcmp(token, "<") == 0)
            continue;
        else if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0)
            break;
        args[ind++] = token;
    }

    args[ind] = NULL;
}

void fg(int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
{
    // executes foreground processes
    int freturn = fork();
    if (freturn != 0)
    {
        childPid = freturn;
    }

    if (freturn == 0)
    {
        // child process

        int fd;
        int stdout_save = dup(1);

        if (*apflag == 1 || *outflag == 1)
        {
            if (*apflag == 1)
            {
                fd = open(outputFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            else
            {
                fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }

            if (fd < 0)
            {
                perror("\033[1;31mFailed to open file\033[0m");
                printf("\x1B[37m");
                return;
            }

            // if >> or > to output file
            // save the current stdin
            // open the file to replace stdout
            // replace standard output with output file
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if (*inflag == 1)
        {
            fd = open(inputFile, O_RDONLY);
            if (fd < 0)
            {
                perror("\033[1;31mInput file does not exist\033[0m");
                printf("\x1B[37m");
                return;
            }
        }

        execvp(args[0], args);
        dup2(stdout_save, STDOUT_FILENO);
        close(stdout_save);
        return;
    }
    else
    {
        wait(NULL); // waiting for the child process to execute
        return;
    }

    childPid = 0;
    return;
}

void bg()
{
    // executes foreground processes
    int freturn = fork();
    if (freturn == 0)
    {
        int exe = execvp(args[0], args);
        if (exe == -1)
            printf("Command does not exist\n");
        return;
    }

    else
    {
        printf("%d\n", freturn);
        back[bind].pid = freturn; // adding the pid of the ongoing process to the struct
        lastinsert(ord, back[bind].comm, freturn, "Running");
        ord++;
        bind++;
    }

    return;
}

void fb(char *cmd, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
{

    *args = (char *)malloc(sizeof(char) * 10000);

    char *copy = strdup(cmd); // creating a duplicate of the command and allocating it memory

    char *remaining = strtok(cmd, ";&|"); // tokenising the command bases on & and ;
    int ind = 0;
    int ampflag = 0, semflag = 0;
    time_t start, end;

    while (remaining)
    {
        if (copy[remaining - cmd + strlen(remaining)] == '&')
        {
            // if the delimiter is &, it must add to the struct and execute it in the background
            ampflag == 1;
            for (int i = 0; i < strlen(remaining); i++)
            {
                if (i == strlen(remaining) - 1)
                {
                    if (remaining[i] == ' ')
                    {
                        remaining[i] = '\0';
                    }
                }
            }

            back[bind].comm = malloc(sizeof(char) * 10000);
            strcpy(back[bind].comm, remaining);
        }

        else
        {
            semflag = 1;
            strcpy(sigcmd, remaining);
        }

        makeCommandArray(remaining);

        if (semflag)
        {
            // execution of foreground process

            time(&start);
            fg(apflag, inflag, outflag, inputFile, outputFile);
            time(&end);
            long long time_taken = end - start;
            if (time_taken > 1 && ampflag == 0)
            {
                printf("took %lld seconds\n", time_taken);
            }
        }

        else
            bg(back);

        remaining = strtok(NULL, "&;");
    }

    free(copy);
    return;
}
