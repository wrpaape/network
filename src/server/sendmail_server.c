#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

extern char **environ;

int
main(int argc,
     char *argv[])
{
        static char *sendmail_argv[] = {
                "sendmail",
                "wrpaape@gmail.com",
                NULL
        };

        int pipe_fds[2];
        pid_t pid;


        for (int i = 1; i < argc; ++i) {

                if (pipe(pipe_fds) < 0)
                        perror("pipe"), exit(1);

                pid = fork();

                if (pid < 0) {
                        perror("fork"), exit(1);

                } else if (pid == 0) {
                        close(pipe_fds[1]);
                        dup2(pipe_fds[0], STDIN_FILENO);

                        execve("/usr/sbin/sendmail",
                               sendmail_argv,
                               environ);
                } else {
                        close(pipe_fds[0]);

                        write(pipe_fds[1],
                              argv[i],
                              strlen(argv[i]));
                }
        }



        return 0;
}
