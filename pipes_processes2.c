#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[2];
  int pipefd2[2];
  int pid;
  pid_t pid2;
  

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};


  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);
  pipe(pipefd2);

  pid = fork();


  if (pid == 0)
    {
      // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe

      dup2(pipefd[0], 0);
      dup2(pipefd2[1], 1);

      // close unused hald of pipe

      close(pipefd[0]);
      close(pipefd[1]);
      close(pipefd2[0]);
      close(pipefd2[1]);

      // execute grep

      execvp("grep", grep_args);
    }
  else
    {
      pid2 = fork();

      if(pid2 == 0){
        dup2(pipefd2[0], 0);

        // close unused hald of pipe

        close(pipefd[0]);
        close(pipefd[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        // execute sort

        execvp("sort", sort_args);
        exit(0);
      }
      // parent gets here and handles "cat scores"

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe
      close(pipefd[0]);
      close(pipefd[1]);
      close(pipefd2[0]);
      close(pipefd2[1]);

      // execute cat

      execvp("cat", cat_args);
    }
}