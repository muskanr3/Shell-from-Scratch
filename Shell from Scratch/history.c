#include "headers.h"

#define HISTORY_MAX_SIZE 20

void addCommand(char *command, char *path)
{
    // adds the command given as input to the file and array

    char history[20][100];
    int history_count = 0;
    char *buf;
    size_t bufsize = 50;
    buf = (char *)malloc(bufsize * sizeof(char));
    FILE *fp;
    char ch;
    int linesCount = 0;

    FILE *f = fopen(path, "a"); // creating the file
    fclose(f);

    FILE *fd = fopen(path, "r");
    // reading all the commands into an array
    if (fd == NULL)
    {
        printf("NULL\n");
    }
    while (fscanf(fd, "%[^\n]%*c", buf) != EOF)
    {
        strcpy(history[linesCount], buf);
        linesCount++;
    }
    fclose(fd);

    if (linesCount >= 20)
    {
        if (strcmp(history[19], command) != 0)
        {
            fd = fopen(path, "w");
            // want to remove first command from file and append the commands one to end
            for (int i = 1; i < 20; i++)
            {
                fprintf(fd, "%s\n", history[i]);
            }
            fclose(fd);

            FILE *fd = fopen(path, "a");
            // append command
            fprintf(fd, "%s", command);
            fclose(fd);
            for (int index = 1; index < 20; index++)
            {
                strcpy(history[index - 1], history[index]);
            }
            strcpy(history[20 - 1], command);
        }
    }
    else
    {
        // if number of commands < 20
        if (strcmp(history[linesCount - 1], command) != 0)
        {
            FILE *fd = fopen(path, "a");
            fprintf(fd, "%s\n", command);
            fclose(fd);

            for (int index = 1; index < linesCount; index++)
            {
                strcpy(history[index - 1], history[index]);
            }
            strcpy(history[19], command);
        }
    }
}

void printHis(char *path, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
{
    char history[20][100];
    int history_count = 0;
    char *buf;
    size_t bufsize = 50;
    buf = (char *)malloc(bufsize * sizeof(char));
    FILE *fp;
    char ch;
    int linesCount = 0;
    FILE *fd = fopen(path, "r");
    // reading all the commands into an array

    int red;
    int stdout_save = dup(1);

    if (*apflag == 1 || *outflag == 1)
    {
        if (*apflag == 1)
        {
            red = open(outputFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            red = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }

        if (red < 0)
        {
            perror("Failed to open file.");
            return;
        }

        // printf("ap = %d, out = %d, in = %d\n", *apflag, *outflag, *inflag);
        // if >> or > to output file
        // save the current stdin
        // open the file to replace stdout
        // replace standard output with output file
        dup2(red, STDOUT_FILENO);
        close(red);
    }

    while (fscanf(fd, "%[^\n]%*c", buf) != EOF)
    {
        strcpy(history[linesCount], buf);
        linesCount++;
    }
    fclose(fd);

    if (linesCount > 10 && linesCount < 20)
    {
        // 10 < lines < 20
        for (int i = linesCount - 10; i < linesCount; i++)
        {
            printf("%s\n", history[i]);
        }
    }
    else if (linesCount > 10)
    {
        // only want to print the last 10 commands
        for (int i = 10; i < 20; i++)
        {
            printf("%s\n", history[i]);
        }
    }

    else
    {
        // want to print all commands
        // if number of lines < 10
        fd = fopen(path, "r");
        while (fscanf(fd, "%[^\n]%*c", buf) != EOF)
        {
            printf("%s\n", buf);
        }
        fclose(fd);
    }

    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);
}
