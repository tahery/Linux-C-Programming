#include "stdlib.h"
#include "stdio.h"
#include "netdb.h"
#include "sys/types.h"
#include "string.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "fcntl.h"

#define MAXDATASIZE 256
#define SERVPORT 4444
#define BACKLOG 10
#define STDIN 0

int main(void)
{
	FILE * fp;
	int sockfd, client_fd;
	int sin_size;
	struct sockaddr_in my_addr, remote_addr;
	char buf[256];
	char buff[256];
	char send_str[256];
	int recvbytes;
	fd_set rfd_set, wfd_set, efd_set;
	struct timeval timeout;
	int ret;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	bzero(&my_addr, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVPORT);
	inet_aton("127.0.0.1", &my_addr.sin_addr);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	sin_size = sizeof(struct sockaddr_in);
	if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) == -1)
	{
		perror("accpet");
		exit(1);
	}

	fcntl(client_fd, F_SETFD, O_NONBLOCK);
	recvbytes = recv(client_fd, buff, MAXDATASIZE, 0);
	buff[recvbytes] = '\0';
	fflush(stdout);

	if ((fp = fopen("name.txt", "a+")) == NULL)
	{
		printf("can not open file, exit\n");
		return -1;
	}
	fprintf(fp, "%s\n", buff);

	while(1)
	{
		FD_ZERO(&rfd_set);
		FD_ZERO(&wfd_set);
		FD_ZERO(&efd_set);
		FD_SET(STDIN, &rfd_set);
		FD_SET(client_fd, &rfd_set);
		FD_SET(client_fd, &wfd_set);
		FD_SET(client_fd, &efd_set);
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
		ret = select(client_fd + 1, &rfd_set, &wfd_set, &efd_set, &timeout);
		if (ret == 0)
		{
			continue;
		}
		if (ret < 0)
		{
			perror("select error: ");
			exit(-1);
		}
		if (FD_ISSET(STDIN, &rfd_set))
		{
			fgets(send_str, 256, stdin);
			send_str[strlen(send_str)-1]='\0';
			if (strncmp("quit", send_str, 4) == 0)
			{
				close(client_fd);
				close(sockfd);
				exit(0);
			}
		}
		send(client_fd, send_str, strlen(send_str), 0);
	}

	if (FD_ISSET(client_fd, &rfd_set))
	{
		recvbytes = recv(client_fd, buf, MAXDATASIZE, 0);
		if (recvbytes == 0)
		{
			close(client_fd);
			close(sockfd);
			exit(0);
		}
		buf[recvbytes] = '\0';
		printf("%s:%s\n", buff, buf);
		printf("Server:\n");
		fflush(stdout);
	}

	if (FD_ISSET(client_fd, &efd_set))
	{
		close(client_fd);
		exit(0);
	}
}