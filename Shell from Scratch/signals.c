#include "headers.h"
#include "cmd.h"
#include "jobs.h"
#include "sig.h"

void sigC()
{
    // killing the current foreground process if ctrl C is called
    if (childPid == 0)
        return;
    int k = kill(childPid, SIGKILL);
    if (k == -1)
        perror("Kill");
}

void sigZ(int sig_num)
{
    if (childPid != 0)
    {
        int k = kill(childPid, SIGSTOP); // stopping the foreground process
        if (k == -1)
            perror("Kill");
        // now need to put the foreground process in jobs linked list to make it a bg process
        lastinsert(ord, sigcmd, childPid, "Stopped");
    }
    return;
}