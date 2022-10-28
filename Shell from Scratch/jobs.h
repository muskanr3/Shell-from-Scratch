#ifndef jobs_h
#define jobs_h

struct node
{
    struct node *next;
    char *comm;
    int pid;
    char *status;
    int order;
};

typedef struct node *node;
struct node *head;

void display();
void lastinsert(int order, char *command, int pid, char *status);
void sortList();
struct node *delete_node(int pid);
void changeStatus(int pid);
void jobs(char *command, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile);
#endif