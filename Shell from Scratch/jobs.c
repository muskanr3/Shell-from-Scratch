#include "headers.h"
#include "jobs.h"

void sortList()
{
    // Node current will point to head
    struct node *current = head, *index = NULL;
    char *temp, *temp_status;
    int temp_pid, temp_order;

    if (head == NULL)
    {
        return;
    }
    else
    {
        while (current != NULL)
        {
            // Node index will point to node next to current
            index = current->next;

            while (index != NULL)
            {
                // If current node's comm is greater than index's node comm, swap the comm between them
                // if (current->comm > index->comm)
                if (strcasecmp(current->comm, index->comm) > 0)
                {
                    temp = current->comm;
                    current->comm = index->comm;
                    index->comm = temp;

                    temp_pid = current->pid;
                    current->pid = index->pid;
                    index->pid = temp_pid;

                    temp_order = current->order;
                    current->order = index->order;
                    index->order = temp_order;

                    temp_status = current->status;
                    current->status = index->status;
                    index->status = temp_status;
                }
                index = index->next;
            }
            current = current->next;
        }
    }
}

void display()
{
    struct node *ptr;
    ptr = head;
    if (ptr == NULL)
    {
        // perror("No background processes currently.");
        return;
    }
    else
    {
        while (ptr != NULL)
        {
            printf("[%d] %s %s [%d]\n", ptr->order, ptr->status, ptr->comm, ptr->pid);
            ptr = ptr->next;
        }
    }
}

void displayR()
{
    // to display the running processes
    struct node *ptr;
    ptr = head;
    if (ptr == NULL)
    {
        // perror("No background processes currently.\n");
        return;
    }
    else
    {
        while (ptr != NULL)
        {
            char status;
            char procfile[10000];

            sprintf(procfile, "/proc/%d/stat", ptr->pid);
            FILE *fd = fopen(procfile, "r");

            fscanf(fd, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*u %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status);

            if (status == 'R' || status == 'S')
            printf("[%d] %s %s[%d]\n", ptr->order, ptr->status, ptr->comm, ptr->pid);
            ptr = ptr->next;
        }
    }
}

void displayS()
{
    // to display the stopped processes

    struct node *ptr;
    ptr = head;
    if (ptr == NULL)
    {
        // perror("No background processes currently.\n");
        return;
    }
    else
    {
        while (ptr != NULL)
        {
            char status;
            char procfile[10000];

            sprintf(procfile, "/proc/%d/stat", ptr->pid);
            FILE *fd = fopen(procfile, "r");

            fscanf(fd, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*u %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status);

            if (status == 'T')
                printf("[%d] %s %s[%d]\n", ptr->order, ptr->status, ptr->comm, ptr->pid);
            ptr = ptr->next;
        }
    }
}

void lastinsert(int order, char *command, int pid, char *status)
{
    // insert node into the end of the linked list
    struct node *ptr, *temp;
    int item;
    ptr = (struct node *)malloc(sizeof(struct node));
    if (ptr == NULL)
    {
        printf("\nOVERFLOW");
    }

    else
    {
        ptr->comm = command;
        ptr->order = order;
        ptr->pid = pid;
        ptr->status = status;

        if (head == NULL)
        {
            ptr->next = NULL;
            head = ptr;
            // printf("struct node inserted\n");
        }
        else
        {
            temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = ptr;
            ptr->next = NULL;
            // printf("struct node inserted\n");
        }
    }
}

struct node *delete_node(int pid)
{
    if (!head)
        return head;
    while (head && head->pid == pid)
        head = head->next;
    struct node *curr = head, *prev = NULL;
    while (curr)
    {
        if (curr->pid == pid)
            prev->next = curr->next;
        else
            prev = curr;
        curr = curr->next;
    }
    return head;
}

void changeStatus(int pid)
{
    struct node *ptr;
    int item, i = 0, flag;
    ptr = head;
    if (ptr == NULL)
    {
        printf("No background processes currently.\n");
    }
    else
    {
        while (ptr != NULL)
        {
            if (ptr->pid == pid)
            {
                // changing the status
                ptr->status = (char *)malloc(sizeof(char) * 10000);
                strcpy(ptr->status, "Stopped");
            }

            ptr = ptr->next;
        }
    }
}

void jobs(char *command, int *apflag, int *inflag, int *outflag, char *inputFile, char *outputFile)
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
            perror("\033[1;31mFailed to open file.\033[0m");
            printf("\x1B[37m");
            return;
        }

        // if >> or > to output file
        // save the current stdin
        // open the file to replace stdout
        // replace standard output with output file
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    char *remaining = strtok_r(command, " ", &command); // tokenising the command bases on & and ;
    int rflag = 0, sflag = 0;
    time_t start, end;

    for (int i = 0; i < 1; i++)
    {
        if (strcmp(command, "-r") == 0)
            rflag = 1;
        else if (strcmp(command, "-s") == 0)
            sflag = 1;
    }

    sortList();

    if (rflag)
        displayR();
    else if (sflag)
        displayS();
    else
        display();

    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);
}
