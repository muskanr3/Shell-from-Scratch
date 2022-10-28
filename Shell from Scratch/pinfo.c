#include "headers.h"

int pinfo(int shellpid, char *command, char *home, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
{
    char *token;
    char *copy = command;
    char cmdfixed[10000];
    strcpy(cmdfixed, command);

    token = strtok_r(copy, " \t", &copy);

    long long int pid;
    long long int tpgid;
    long long int pgrp;

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
            return 0;
        }

        // printf("ap = %d, out = %d, in = %d\n", *apflag, *outflag, *inflag);
        // if >> or > to output file
        // save the current stdin
        // open the file to replace stdout
        // replace standard output with output file
        dup2(red, STDOUT_FILENO);
        close(red);
    }

    char status;
    long unsigned memory;
    char executable[10000];
    char execFile[10000];
    char procfile[10000];
    if (strcmp(copy, "") == 0)
    {
        // return pinfo for the current shell
        // printf("pid : %d\n", shellpid);
        sprintf(procfile, "/proc/%d/stat", shellpid);
        sprintf(execFile, "/proc/%d/exe", shellpid);
        // Now look for the pid in the proc file
    }

    else
    {
        // return pinfo for the given file
        sprintf(procfile, "/proc/%s/stat", copy);
        sprintf(execFile, "/proc/%s/exe", copy);
    }

    // opening the files
    FILE *fd = fopen(procfile, "r");
    FILE *fd1 = fopen(execFile, "r");

    if (fd == NULL || fd1 == NULL)
    {
        if (apflag == 0 && outflag == 0)
        {
            // only write to terminal if there is no output file
            perror("\033[1;31mProcess does not exist\033[0m");
            printf("\x1B[37m");
        }

        else
        {
            // write to the file
            printf("Process does not exist.\n");
        }

        dup2(stdout_save, STDOUT_FILENO);
        close(stdout_save);
        return 0;
    }

    else
    {
        fscanf(fd, "%lld %*s %c %*d %lld %*d %*d %lld %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &pid, &status, &pgrp, &tpgid, &memory);
        fclose(fd);
        printf("pid : %lld\n", pid);

        readlink(execFile, executable, 10000);
        if (pgrp == tpgid)
        {
            printf("process status : %c", status);
            printf("+\n");
        }
        else
        {
            printf("process status : %c\n", status);
        }
        printf("memory : %lu\n", memory);

        char *tldr = "~";
        char buffer[10000];
        char *remain = strstr(executable, home); // returns first occurance of the substring home
        // getting relative path
        if (remain)
        {
            int diffbytes = remain - executable;
            strncpy(buffer, executable, diffbytes);
            sprintf(buffer + (remain - executable), "%s%s", tldr, remain + strlen(home));
        }
        printf("executable path : %s\n", buffer);
    }

    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);
    return 0;
}