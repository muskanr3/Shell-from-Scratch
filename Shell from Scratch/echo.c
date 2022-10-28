#include "headers.h"
#include "cmd.h"

// displays the input string on the terminal
void echo(char *str, char *token, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
{
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
            perror("\033[1;31mFailed to open file.\033[0m");
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

    while (str != NULL)
    {
        // printf("str - %s\n", str);
        // printf("token - %s\n", token);

        str = strtok_r(token, " \t\n;&", &token);

        if (str != NULL)
        {
            if (strcmp(str, "echo") == 0)
                continue;
            printf("%s ", str);
        }
    }
    printf("\n");

    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);
}