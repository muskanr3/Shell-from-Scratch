#include "headers.h"

int dotcount = 0, dircount = 1;
char dirarr[100][100];
int printflag = 0;

// display all files and folders of directory
int dis(char *cmd, char *file)
{
    char commands[1000][100];
    // open current directory
    struct stat s;
    DIR *d;
    struct dirent *dir;
    d = opendir(cmd);
    int i = 0;
    strcpy(dirarr[0], ".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(file, "\0") != 0)
            {
                // printf("in here %s\n", file);
                // printf("dir name %s\n", dir->d_name);

                if (strcmp(".", dir->d_name) == 0 || strcmp("..", dir->d_name) == 0)
                    continue;
                char dispath[10000];
                strcat(dispath, cmd);
                strcat(dispath, "/");
                strcat(dispath, dir->d_name);

                if (strcmp(dir->d_name, file) == 0)
                {
                    printf("%s\n", dispath);
                    printflag = 1;
                }
                dis(dispath, file);
                memset(dispath, '\0', 10000); // resetting the array
            }

            else
            {
                // building file path name
                char dispath[10000];
                strcat(dispath, cmd);
                strcat(dispath, "/");
                strcat(dispath, dir->d_name);

                if (strcmp(dir->d_name, "..") == 0)
                {
                    // hidden files not to be printed
                    memset(dispath, '\0', 10000);
                    continue;
                }
                if (strcmp(dir->d_name, ".") == 0 && dotcount == 0)
                {
                    // current directory only to be displayed once
                    printf(".\n");
                    dotcount++;
                    memset(dispath, '\0', 10000);
                    continue;
                }
                else if (strcmp(dir->d_name, ".") == 0 && dotcount > 0)
                {
                    memset(dispath, '\0', 10000);
                    continue;
                }
                strcpy(commands[i++], dir->d_name);
                stat(commands[i], &s);

                dis(dispath, file);
                printf("%s\n", dispath);
                memset(dispath, '\0', 10000); // resetting the array
            }
        }
        dircount--;
    }
    closedir(d);
    return 0;
}

// for displaying only directories
int disD(char *path, char *file)
{
    char commands[1000][100];
    struct stat s;
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    int i = 0;
    strcpy(dirarr[0], ".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {

            if (strcmp(file, "\0") != 0)
            {

                if (strcmp(".", dir->d_name) == 0 || strcmp("..", dir->d_name) == 0)
                    continue;
                char dispath[10000];
                strcat(dispath, path);
                strcat(dispath, "/");
                strcat(dispath, dir->d_name);

                if (strcmp(path, file) == 0)
                {
                    printflag = 1;
                    printf("%s\n", dispath);
                }
                disD(dispath, file);
                memset(dispath, '\0', 10000); // resetting the array
            }
            else
            {
                // buiding file path name
                char dispath[10000];
                strcat(dispath, path);
                strcat(dispath, "/");
                strcat(dispath, dir->d_name);

                if (strcmp(dir->d_name, "..") == 0)
                {
                    // hidden files not to be printed
                    memset(dispath, '\0', 10000);
                    continue;
                }
                if (strcmp(dir->d_name, ".") == 0 && dotcount == 0)
                {
                    // current directory only to be displayed once
                    printf(".\n");
                    dotcount++;
                    memset(dispath, '\0', 10000);
                    continue;
                }
                else if (strcmp(dir->d_name, ".") == 0 && dotcount > 0)
                {
                    memset(dispath, '\0', 10000);
                    continue;
                }
                strcpy(commands[i++], dir->d_name);
                stat(commands[i], &s);

                disD(dispath, file);
                if (opendir(dispath) != NULL)
                    printf("%s\n", dispath);
                memset(dispath, '\0', 10000); // resetting the array
            }
        }
        dircount--;
    }
    closedir(d);
    return 0;
}

// for displaying only files
int disF(char *path, char *file)
{
    char commands[1000][100];
    struct stat s;
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    int i = 0;
    strcpy(dirarr[0], ".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {

            if (strcmp(file, "\0") != 0)
            {

                if (strcmp(".", dir->d_name) == 0 || strcmp("..", dir->d_name) == 0)
                    continue;
                char dispath[10000];
                strcat(dispath, path);
                strcat(dispath, "/");
                strcat(dispath, dir->d_name);

                if (strcmp(dir->d_name, file) == 0)
                {
                    printflag = 1;
                    printf("%s\n", dispath);
                }
                disF(dispath, file);
                memset(dispath, '\0', 10000); // resetting the array
            }
            else
            {
                // buiding file path name
                char dispath[10000];
                strcat(dispath, path);
                strcat(dispath, "/");
                strcat(dispath, dir->d_name);

                if (strcmp(dir->d_name, "..") == 0)
                {
                    // hidden files not to be printed
                    memset(dispath, '\0', 10000);
                    continue;
                }
                if (strcmp(dir->d_name, ".") == 0)
                {
                    memset(dispath, '\0', 10000);
                    continue;
                }

                strcpy(commands[i++], dir->d_name);
                stat(commands[i], &s);

                disF(dispath, file);
                if (opendir(dispath) == NULL)
                    printf("%s\n", dispath);
                memset(dispath, '\0', 10000); // resetting the array
            }
        }
        dircount--;
    }
    closedir(d);
    return 0;
}

// driver code for discover
void discover(char *cmd, char *home, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
{
    char *token;
    char *copy = cmd;
    char cmdfixed[10000];
    strcpy(cmdfixed, cmd);
    int dflag = 0;
    int fflag = 0;
    char file[10000];
    char tokarr[1000][1000];
    printflag = 0;
    char savepath[10000];
    getcwd(savepath, 10000);

    char *p;
    char *q = "\"";
    p = strstr(cmdfixed, q);
    if (p != NULL)
    {
        strcpy(tokarr[4], file);
        strncat(file, &p[1], strlen(p) - 2);
        strcpy(tokarr[4], file);
    }

    int index = 0;
    while ((token = strtok_r(copy, " \t", &copy)))
    {
        // printf("token = %s\n", token);
        if (strcmp(token, "discover") == 0)
            strcpy(tokarr[0], "discover");
        else if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0 || strcmp(token, ">>") == 0)
            break;
        else if (strcmp(token, "-d") == 0)
            strcpy(tokarr[2], token);
        else if (strcmp(token, "-f") == 0)
            strcpy(tokarr[3], token);
        else if ((strcmp(token, "discover") != 0 || strcmp(token, "-d") != 0 || strcmp(token, "-f") != 0) && token[0] != '"')
            strcpy(tokarr[1], token);
    }

    if (strcmp(tokarr[1], "~") == 0)
    {
        chdir(home);
        strcpy(tokarr[1], home);
    }

    if (strcmp(tokarr[2], "-d") == 0)
    {
        dflag = 1;
    }

    if (strcmp(tokarr[3], "-f") == 0)
    {
        fflag = 1;
    }

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

        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    if (strcmp(cmdfixed, "discover -d") == 0)
    {
        disD(".", file);
        // memset(tokarr, '\0', sizeof(tokarr[0][0] * 1000 * 1000));
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                tokarr[i][j] = '\0';
            }
        }

        memset(file, '\0', 10000);
        dup2(stdout_save, STDOUT_FILENO);
        close(stdout_save);
        return;
    }
    else if (strcmp(cmdfixed, "discover ~") == 0)
    {
        // discover ~
        dis(tokarr[1], file);
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                tokarr[i][j] = '\0';
            }
        }

        memset(file, '\0', 10000);
        dup2(stdout_save, STDOUT_FILENO);
        close(stdout_save);
        return;
    }

    else if (strcmp(cmdfixed, "discover -f") == 0)
    {
        disF(".", file);
        // memset(tokarr, '\0', sizeof(tokarr[0][0] * 1000 * 1000));
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                tokarr[i][j] = '\0';
            }
        }

        memset(file, '\0', 10000);
        dup2(stdout_save, STDOUT_FILENO);
        close(stdout_save);
        return;
    }

    else if (strcmp(tokarr[1], "\0") == 0 && strcmp(tokarr[2], "-d") == 0 && strcmp(tokarr[3], "-f") == 0)
    {
        // discover -d -f
        // directory is null

        dis(".", file);
        // memset(tokarr, '\0', sizeof(tokarr[0][0] * 1000 * 1000));
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                tokarr[i][j] = '\0';
            }
        }

        memset(file, '\0', 10000);
        dup2(stdout_save, STDOUT_FILENO);
        close(stdout_save);
        return;
    }

    else if (strcmp(tokarr[1], "\0") == 0 && strcmp(tokarr[2], "\0") == 0 && strcmp(tokarr[3], "\0") == 0)
    {
        // discover
        // directory is null

        dis(".", file);
        // memset(tokarr, '\0', sizeof(tokarr[0][0] * 1000 * 1000));
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                tokarr[i][j] = '\0';
            }
        }

        memset(file, '\0', 10000);
        dup2(stdout_save, STDOUT_FILENO);
        close(stdout_save);
        return;
    }

    else if (strcmp(tokarr[1], "\0") != 0 && strcmp(tokarr[2], "\0") == 0 && strcmp(tokarr[3], "\0") == 0)
    {
        // discover <folder> <file>
        dis(tokarr[1], file);
    }

    else if (strcmp(tokarr[1], "\0") == 0 && dflag)
    {
        // discover -d <file>
        disD(".", file);
    }

    else if (strcmp(tokarr[1], "\0") == 0 && fflag)
    {
        // discover -f <file>
        disF(".", file);
    }

    else if (dflag)
    {
        // discover <folder> -d
        disD(tokarr[1], file);
    }

    if (fflag)
    {
        // discover <folder> -f
        disF(tokarr[1], file);
    }

    // memset(tokarr, '\0', sizeof(tokarr[0][0] * 1000 * 1000));
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            tokarr[i][j] = '\0';
        }
    }

    memset(file, '\0', 10000);

    if (printflag == 0)
        printf("File not found.\n");

    chdir(savepath);

    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);

    return;
}