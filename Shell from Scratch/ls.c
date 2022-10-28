#include "headers.h"
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void bubbleSort(char names[][100], int nonames)
{
    int swap_count = 0;
    char tempstr[100];

    if (nonames == 1)
        return;

    for (int i = 0; i < nonames - 1; i++)
    {
        if (strcasecmp(&names[i][0], &names[i + 1][0]) < 0)
        {
            continue;
        }
        else
        {
            strcpy(tempstr, &names[i][0]);
            strcpy(&names[i][0], &names[i + 1][0]);
            strcpy(&names[i + 1][0], tempstr);
            swap_count++;
        }
    }
    if (swap_count > 0)
        bubbleSort(names, nonames - 1);
}

void ls(char *op, char *home, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
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
            perror("Failed to open file.");
            return;
        }

        // if >> or > to output file
        // save the current stdin
        // open the file to replace stdout
        // replace standard output with output file
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    struct stat s;
    char commands[1000][100];
    char savecwd[10000];
    getcwd(savecwd, 10000);

    // printf("op = %s\n", op);
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    int i = 0;
    char opcopy[10000];
    strcpy(opcopy, op);

    char *flag1;
    char *token = strtok_r(op, " ", &flag1);
    char *flag = strtok_r(flag1, " ", &flag);
    if (flag == NULL)
    {
        flag = malloc(sizeof(char) * 10000);
        strcpy(flag, "no flag");
    }
    stat(flag, &s);

    // printf("flag = %s\n", flag);

    // if flag is not -a then don't print hidden files
    if (flag != NULL && strcmp(flag, "..") == 0)
    {
        // need to get files from parent directory
        d = opendir("..");
    }
    else if (flag != NULL && strcmp(flag, "~") == 0)
    {
        // need to get files from root
        d = opendir(home);
    }
    else if (flag != NULL && opendir(flag) != NULL)
    {
        // ls <directory_path>

        if (opendir(flag) == NULL)
        {
            perror("Directory does not exist.");
            return;
        }

        // changing the directory
        d = opendir(flag);
    }

    else if (flag != NULL && stat(flag, &s) == 0)
    {
        // ls <filename>
        printf("%s\n", flag);
        return;
    }

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(opcopy, "ls") == 0 && dir->d_name[0] == '.' && strcmp(flag, "-a") != 0 && strcmp(flag, "-a -l") != 0 && strcmp(flag, "-al") != 0 && strcmp(flag, "-la") != 0 && strcmp(flag, "-l -a") != 0)
            {
                continue;
            }
            strcpy(commands[i++], dir->d_name);
        }
        closedir(d);
    }

    bubbleSort(commands, i); // sorting the commands in alphabetical order

    for (int j = 0; j < i; j++)
    {
        stat(commands[j], &s);

        char buff[20];
        struct passwd pwd;
        struct passwd *password;
        struct group grp;
        struct group *group;
        char buf[1024];
        char buf1[1024];

        if (flag != NULL && (strcmp(flag, "-l") == 0 || strcmp(flag, "-a -l") == 0 || strcmp(flag, "-al") == 0 || strcmp(flag, "-la") == 0 || strcmp(flag, "-l -a") == 0))
        {
            lstat(commands[j], &s);
            strftime(buff, sizeof(buff), "%b %d %H:%M", localtime(&s.st_atime));
            if (S_ISDIR(s.st_mode))
                printf("d");
            else
                printf("-");
            if (s.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");
            if (s.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");
            if (s.st_mode & S_IXUSR)
                printf("w");
            else
                printf("-");
            if (s.st_mode & S_IRGRP)
                printf("w");
            else
                printf("-");
            if (s.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");
            if (s.st_mode & S_IXGRP)
                printf("w");
            else
                printf("-");
            if (s.st_mode & S_IROTH)
                printf("w");
            else
                printf("-");
            if (s.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");
            if (s.st_mode & S_IXOTH)
                printf("w");
            else
                printf("-");
            getpwuid_r(s.st_uid, &pwd, buf, sizeof(buf), &password);
            getgrgid_r(s.st_gid, &grp, buf1, sizeof(buf1), &group);
            printf("%3ld %s %s %5ld %10s ", s.st_nlink, pwd.pw_name, grp.gr_name, s.st_size, buff);
        }

        if (*apflag == 1 || *inflag == 1 || *outflag == 1)
        {
            if (S_ISDIR(s.st_mode))
                printf("%s\n", commands[j]);
            else if (s.st_mode & S_IXUSR)
                printf("%s\n", commands[j]);
            else
                printf("%s\n", commands[j]);
        }
        else
        {
            if (S_ISDIR(s.st_mode))
                printf("\033[;34m%s\033[0m\n", commands[j]);
            else if (s.st_mode & S_IXUSR)
                printf("\033[;32m%s\033[0m\n", commands[j]);
            else
                printf("%s\n", commands[j]);
        }
    }

    // changing back to original directory
    chdir(savecwd);

    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);
}
