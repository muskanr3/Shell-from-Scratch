#ifndef cmd_h
#define cmd_h

void echo(char *str, char *token, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
void pwd(int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
void cdDotDot(char *path);
void cdDot(char *path);
void cdFile(char *path);
void printHis(char *path, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
void addCommand(char *command, char *path);
void bubbleSort(char names[][100], int nonames);
void ls(char *op, char *home, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
void discover(char *cmd, char *home, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
int disF(char *path, char *file);
int disD(char *path, char *file);
int dis(char *cmd, char *file);
int pinfo(int shellpid, char *cmd, char *home, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
void fb(char *cmd, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
void bg();
void fg(int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
void makeCommandArray(char *cmd);
void check();
int checkSymbol(char *cmd, int *apflag, int *inflag, int *outflag, char *command, char *inputFile, char *outputFile);
void executePipe(char *cmd, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
void makeArray(char *cmd);
#endif