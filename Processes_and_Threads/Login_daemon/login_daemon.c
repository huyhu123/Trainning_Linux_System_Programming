#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pwd.h>
#include <time.h>
#include <utmp.h>

void sigterm_handler(int signum)
{
    exit(0);
}

int main(int argc, char *argv[])
{
    pid_t pid, sid;

    // Fork off the parent process
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Change the current working directory
    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Close the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Open the log file for writing
    int log_fd = open("/var/log/login_daemon.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (log_fd == -1)
    {
        exit(EXIT_FAILURE);
    }

    // Register the SIGTERM signal handler
    signal(SIGTERM, sigterm_handler);

    // Main loop
    while (1)
    {
        struct utmp *ut;
        setutent();
        while ((ut = getutent()) != NULL)
        {
            if (ut->ut_type == USER_PROCESS)
            {
                struct passwd *pw = getpwnam(ut->ut_user);
                if (pw != NULL)
                {
                    time_t now = time(NULL);
                    char *timestamp = ctime(&now);
                    timestamp[strlen(timestamp) - 1] = '\0'; // Remove newline character
                    dprintf(log_fd, "%s: User %s logged in\n", timestamp, pw->pw_name);
                }
            }
        }
        endutent();
        sleep(60);
    }

    // Close the log file
    close(log_fd);

    // Exit the child process
    exit(EXIT_SUCCESS);
}