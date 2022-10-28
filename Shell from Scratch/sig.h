#ifndef sig_h
#define sig_h
void sig(char *cmd);
int childPid;
int ord; // index of the jobs
char sigcmd[10000];
void sigC();
void sigZ();
#endif