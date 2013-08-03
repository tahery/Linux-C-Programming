#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main()
{
	pid_t pid1, pid2;
	int num;
	int pipe1[2], pipe2[2];
	// make it pipe
	pipe(pipe1);
	pipe(pipe2);
	// let's fork
	pid1 = fork();
	pid2 = fork();
	char buf1[100];
	char buf2[100];
	// initialize the char
	memset(buf1, 0, sizeof(buf1));
	memset(buf2, 0, sizeof(buf2));
	if (pid1 != 0 && pid2 != 0)	//main process
	{
		printf("main PID:%d\n", getpid());
		printf("I am main process\n");
		// close the pipe 
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
		// wait the child process
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
	}
	if (pid1 == 0 && pid2 != 0)	//p1
	{
			int pid3;
			// another fork
			pid3 = fork();
			if (pid3 != 0)
			{
				printf("I am child process p1\n");
				printf("p1 PID:%d ", getpid());
				printf("PPID:%d\n", getppid());
				// write to pipe1
				close(pipe1[0]);
				write(pipe1[1], "Child process p1 is sending a message!", 50);
				close(pipe1[1]);
				// read from pipe2
				close(pipe2[1]);
				if((num = read(pipe2[0], buf1, 50)) >0){
					printf("%s\n", buf1);
				}
				close(pipe2[0]);
				// wait for its child process
				waitpid(pid3, NULL, 0);
			}
			else	//p3
			{
				printf("I am child process p3\n");
				printf("p3 PID:%d ", getpid());
				printf("PPID:%d\n", getppid());
				// execl the ls
				execl("/bin/ls", "ls", "-1", NULL);
			}
	}
	if (pid1 != 0 && pid2 == 0)	//p2
	{
		printf("I am child process p2\n");
		printf("p2 PID:%d ", getpid());
		printf("PPID:%d\n", getppid());
		// read from pipe1
		close(pipe1[1]);
		if((num = read(pipe1[0], buf1, 50)) >0){
			printf("%s\n", buf1);
		}
		close(pipe1[0]);
		// write to pipe2
		close(pipe2[0]);
		write(pipe2[1], "Child process p2 is sending a message!", 50);
		close(pipe2[1]);
	}
	if (pid1 == 0 && pid2 == 0) //unknown
	{
		// close the pipe
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
	}
	return 0;
}