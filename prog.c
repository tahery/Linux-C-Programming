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
	int pipe1[2];
	pipe(pipe1);
	pid1 = fork();
	pid2 = fork();
	char buf1[100];
	memset(buf1, 0, sizeof(buf1));
	if (pid1 != 0 && pid2 != 0)	//main process
	{
		printf("main PID:%d\n", getpid());
		printf("I am main process\n");
		close(pipe1[0]);
		close(pipe1[1]);
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
	}
	if (pid1 == 0 && pid2 != 0)	//p1
	{
			int pid3;
			pid3 = fork();
			if (pid3 != 0)
			{
			printf("I am child process p1\n");
			printf("p1 PID:%d\n", getpid());
			printf("PPID:%d\n", getppid());
			close(pipe1[0]);
			if(write(pipe1[1], "Child process p1 is sending a message!", 50) != -1)
				printf("write success!\n");
			else
				printf("write failed");
			close(pipe1[1]);
			}
			else
			{
				printf("I am child process p3\n");
				printf("p3 PID:%d\n", getpid());
				printf("PPID:%d\n", getppid());
			}
	}
	if (pid1 != 0 && pid2 == 0)	//p2
	{
		printf("I am child process p2\n");
		printf("p2 PID:%d\n", getpid());
		printf("PPID:%d\n", getppid());
		close(pipe1[1]);
		if((num = read(pipe1[0], buf1, 50)) >0){
			printf("%d %s\n", num, buf1);
		}
		close(pipe1[0]);
	}
	if (pid1 == 0 && pid2 == 0) //p3
	{
		close(pipe1[0]);
		close(pipe1[1]);
	}
	return 0;
}