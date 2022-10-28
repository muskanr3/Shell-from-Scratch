#include "headers.h"
#include "cmd.h"

// returns current working directory
void pwd(int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
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
            perror("\033[1;31mFailed to open output file.\033[0m");
            printf("\x1B[37m");
            return;
        }


        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    char d[1000];
    getcwd(d, 1000);
    printf("%s\n", d);
    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);
}