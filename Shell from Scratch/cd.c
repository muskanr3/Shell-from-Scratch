#include "headers.h"
#include "cmd.h"
// changes directory

void cdDotDot(char *path)
{
    chdir("..");
}

void cdDot(char *path)
{
    chdir(".");
}

void cdFile(char *path)
{
    chdir(path);
}
