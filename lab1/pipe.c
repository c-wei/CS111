#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	// No arguments provided
	if(argc <= 1){
		errno = EINVAL;
		perror("ERROR: Must provide at least 1 argument.");
		exit(errno);
	}

	// 1 argument provided -- just execute the individual program
	if(argc == 2){
		if(execlp(argv[1], argv[1], NULL) == -1){
			perror("ERROR: Error executing program.");
			exit(errno);
		}
		return 0;
	}

	// 2+ arguments provided -- implement piping
	if(argc > 2){
		/*
		1. parse & execute arg[1], arg[2], ...
		2. fork/exec to run each program in a new process
		3. create pipes between adjacent processes using dup2
		*/
		int fd1[2];
		int fd2[2];
		
		for(int i = 1; i < argc; i++){

			if (i < argc - 1){
				// Check for pipe error:
				if(pipe(fd2) == -1){
					perror("ERROR: Pipe error.");
					exit(errno);
				}
			}
			
			// Attempt to fork
			pid_t pid = fork();
			pid = fork();
			// Fork error:
			if (pid == -1){
				perror("ERROR: Fork error.");
				exit(errno);
			}

			// Child Process:
			else if(pid == 0) 
			{
				if (i > 1){
					dup2(fd1[0],0); // read from prev pipe's read end
					close(fd1[0]);
					close(fd1[1]);
				}
				if(i < argc-1){
					dup2(fd2[1], 1); // write to current pipe's write end
					close(fd2[0]);
					close(fd2[0]);
				}
	
				if(execlp(argv[i], argv[i], NULL) == -1){
					perror("ERROR: Error executing program.");
					exit(errno);
				}
				return 0;
			}
			// Parent Process:
			else
			{
				int wstatus;
				waitpid(pid, &wstatus, 0);
				if(!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0){
					exit(WEXITSTATUS(wstatus));
				}

				close(fd2[1]);
				if(i < argc-1){
					fd1[0] = fd2[1];
					fd1[1] = fd2[1];
				}
				else{
					close(fd1[0]);
					close(fd1[1]);
					close(fd2[0]);
					close(fd2[1]);
				}

			}
		}
	}
	return 0;
}
