#include "headers.h"
#include "cmd.h"

int checkSymbol(char *cmd, int *apflag, int *inflag, int *outflag, char *command, char *inputFile, char *outputFile)
{

    int i = 0;
    for (; cmd[i] != '\0'; i++)
    {
        if (cmd[i] == '\t')
            continue;
        if (cmd[i] == '>')
        {
            *outflag = 1;
            if (cmd[i + 1] == '>')
            {
                *apflag = 1;
                i++;
            }
            break;
        }
        else if (cmd[i] == '<')
        {
            *inflag = 1;
            break;
        }

        command[i] = cmd[i];
    }
    // have copied input file and symbol
    // remove space from last letter of input file

    if (command[i - 1] == ' ')
    {
        command[i - 1] = '\0';
    }
    // printf("cmd = %s\n", command);

    if (*inflag == 1)
    {
    in:;

        int j = 0;
        for (i = i + 1; cmd[i] != '\0'; i++)
        {
            if (cmd[i] == '>')
            {
                *outflag = 1;
                goto out;
            }
            if (cmd[i] == ' ')
            {
                continue;
            }
            if (cmd[i] == '\t')
                continue;
            inputFile[j++] = cmd[i];
        }

        inputFile[i - 1] = '\0';
    }

    else if (*outflag == 1 || *apflag == 1)
    {
    out:;
        int j = 0;
        for (i = i + 1; cmd[i] != '\0'; i++)
        {
            if (cmd[i] == '<')
            {
                *inflag = 1;
                goto in;
            }
            if (cmd[i] == ' ')
            {
                continue;
            }
            if (cmd[i] == '\t')
                continue;
            outputFile[j++] = cmd[i];
        }
    }
    outputFile[i - 1] = '\0';

    // printf("input file = %s\n", inputFile);
    // printf("output file = %s\n", outputFile);

    return 0;
}
