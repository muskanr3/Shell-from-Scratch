#include "headers.h"
#include "jobs.h"

void backToForeground(char *cmd)
{
    // brings a background process to execute in the foreground

    // tokenising the string
    char *token;
    char *copy = cmd;
    char cmdfixed[10000];
    strcpy(cmdfixed, cmd);
    int argcount = 0, job, flag = 0;

    while ((token = strtok_r(copy, " \t", &copy)))
    {
        argcount++;
        if (strcmp(token, "fg") == 0)
            continue;
        else if (argcount == 2)
            job = atoi(token);
    }

    if (argcount > 2)
    {
        // too many arguments in bg call
        printf("Too many arguments in bg call.\n");
    }

    else if (argcount < 2)
    {
        // too few arguments in bg call
        printf("Too few arguments in bg call.\n");
    }

    else
    {
        struct node *ptr;
        ptr = head;
        int flag = 0;
        int pid;
        if (ptr == NULL)
        {
            printf("No background processes currently.\n");
        }
        else
        {
            while (ptr != NULL)
            {
                if (ptr->order == job)
                {
                    pid = ptr->pid;
                    flag = 1;
                    break;
                }
                ptr = ptr->next;
            }

            if (flag != 1)
                printf("The given job does not exist.\n");
        }

        // setting the signals to their default values
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);

        // bringing the background process to the foreground
        int t = tcsetpgrp(0, pid);
        if (t == -1)
            perror("Tcsetpgrp");

        // telling the process to start executing again in the foreground
        int k = kill(pid, SIGCONT);
        if (k == -1)
            perror("Kill");

        // parent waiting for process to execute
        int stat;
        int w = waitpid(pid, &stat, WUNTRACED);
        if (w == -1)
            perror("Kill");

        // giving control of the foreground back to the terminal
        int t1 = tcsetpgrp(0, getpgrp());
        if (t1 == -1)
            perror("Tcsetpgrp");

        // setting the signals to their default values
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
    }
}

void backgrd(char *cmd)
{
    char *token;
    char *copy = cmd;
    char cmdfixed[10000];
    strcpy(cmdfixed, cmd);
    int argcount = 0, job, signal, flag = 0;

    while ((token = strtok_r(copy, " \t", &copy)))
    {
        argcount++;
        if (strcmp(token, "bg") == 0)
            continue;
        else if (argcount == 2)
            job = atoi(token);
    }

    if (argcount > 2)
    {
        // too many arguments in bg call
        printf("Too many arguments in bg call.\n");
    }

    else if (argcount < 2)
    {
        // too few arguments in bg call
        printf("Too few arguments in bg call.\n");
    }

    else
    {
        struct node *ptr;
        ptr = head;
        int flag = 0;
        if (ptr == NULL)
        {
            printf("No background processes currently.\n");
        }
        else
        {
            while (ptr != NULL)
            {
                if (ptr->order == job)
                {
                    int pid = ptr->pid;
                    int k = kill(pid, SIGCONT);
                    if (k == -1)
                        perror("Kill");
                    flag = 1;
                }
                ptr = ptr->next;
            }

            if (flag != 1)
                printf("The given job does not exist.\n");
        }
    }
}
