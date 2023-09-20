#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
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
    syslog(LOG_INFO, "Received SIGTERM signal, exiting...");
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

    // Change the file mode mask
    umask(0);

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

    // Open the system log
    openlog("login_daemon", LOG_PID, LOG_USER);

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
                struct passwd *pw = getpwuid(ut->ut_user);
                if (pw != NULL)
                {
                    syslog(LOG_INFO, "User %s logged in at %s", pw->pw_name, ctime(&ut->ut_tv.tv_sec));
                }
            }
        }
        endutent();
        sleep(60);
    }

    // Close the system log
    closelog();

    // Exit the child process
    exit(EXIT_SUCCESS);
}