#include "headers.h"
#include "jobs.h"
#include <signal.h>

void sig(char *cmd)
{
    char *token;
    char *copy = cmd;
    char cmdfixed[10000];
    strcpy(cmdfixed, cmd);
    int argcount = 0, job, signal, flag = 0;

    while ((token = strtok_r(copy, " \t", &copy)))
    {
        argcount++;
        if (strcmp(token, "sig") == 0)
            continue;
        else if (argcount == 2)
            job = atoi(token);
        else if (argcount == 3)
            signal = atoi(token);
    }

    if (argcount > 3)
    {
        // too many arguments in sig call
        perror("Too many arguments in sig call.\n");
    }

    else if (argcount < 3)
    {
        // too few arguments in sig call
        perror("Too few arguments in sig call.\n");
    }

    else
    {
        // number of arguments is 3
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
                if (ptr->order == job)
                {
                    kill(ptr->pid, signal);
                    if (signal == 1)
                        delete_node(ptr->pid);
                    flag = 1;
                    break;
                }
                ptr = ptr->next;
            }

            if (flag != 1)
                printf("The given job does not exist.\n");
        }
    }
}