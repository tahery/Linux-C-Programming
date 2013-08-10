#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "string.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "sys/wait.h"
#define SERVPORT 3333
#define BACKLOG 10

int main()
{
	int sockfd, client_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	int sin_size;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0 == -1)))
	{
		perror("socket creating ERROR");
		exit(1);
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);
	if (bind(sockfd, (struct sockaddr * ) &my_addr, sizeof(struct sockaddr)) == -1) 
	{
		perror("bind ERROR!");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen ERROR!");
		exit(1);
	}
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if ((client_fd = accept(sockfd, (struct sockaddr *) &remote_addr, &sin_size)) == -1)
		{
			perror("accept ERROR");
			continue;
		}
		printf("receive a connection from: %d\n", inet_ntoa(remote_addr.sin_addr));
		if (!fork())
		{
			if (send(client_fd, "connected\n", 26, 0) == -1)
				perror("send ERROR");
			close(client_fd);
			exit(0);
		}
		close(client_fd);
	}
}