#include "headers.h"
#include "jobs.h"
#include "cmd.h"
char *args[10000]; // array of commands for execvp

void makeArray(char *cmd)
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

void executePipe(char *cmd, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
{
    char *token;
    int originalIn = STDIN_FILENO;
    int originalOut = STDOUT_FILENO;
    int noOfPipes = 0;
    for (int i = 0; cmd[i] != '\0'; i++)
    {
        if (cmd[i] == '|')
        {
            noOfPipes++;
        }
    }
    int numCommands = noOfPipes + 1;
    char *copy = strdup(cmd);           // creating a duplicate of the command and allocating it memory
    char *remaining = strtok(cmd, "|"); // tokenising the command bases of |
    int pipes[noOfPipes][2];

    char *pipe_arr[noOfPipes + 1];
    int args = 0;

    for (int i = 0; i < numCommands; i++)
    {
        if (noOfPipes >= 1)
        {

            char *remaining = strtok(cmd, "|"); // tokenising the command bases on & and ;

            while (remaining)
            {
                printf("rem = %s\n", remaining);
                printf("cmd = %s\n", cmd);

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

                pipe_arr[args] = (char *)malloc(sizeof(char) * ((strlen(remaining) + 1)));
                strcpy(pipe_arr[args], remaining);
                remaining = strtok(NULL, "|");
                args++;
            }
            pipe_arr[args] = NULL;
            pipe(pipes[i]);

            if (i == 1)
            {
                // first process needs to read from terminal
                // dup2(pipes[0][0], 0);
                dup2(pipes[0][1], STDOUT_FILENO); // changing the std output to write end
            }

            if (i > 1 && i != (numCommands - 1))
            {
                // want to read input from the output from the previous process
                dup2(pipes[i - 1][0], STDIN_FILENO);
                // want to change std output to output of the process
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            if (i == (numCommands - 1))
            {
                // want to read input from the output from the previous process
                dup2(pipes[i - 1][0], STDIN_FILENO);
                // last process should output on terminal
                dup2(originalOut, STDOUT_FILENO);
            }

            token = pipe_arr[i];
        }
        return;
    }
}