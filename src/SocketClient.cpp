#include <stdio.h>
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;

#define PORT 1234
#define MAXDATASIZE 100

int main(int argc, char *argv[]) {
	int fd;
	char buf[MAXDATASIZE];
	char rvbuf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in server;

	if (argc != 2) {
		printf("ip%s\n", argv[1]);
		exit(-1);
	}

	if ((he = gethostbyname(argv[1])) == NULL) {
		printf("gethostbyname() error");
		exit(1);
	}

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket() error.");
		exit(1);
	}

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
//	server.sin_addr = *((struct in_addr *) he->h_addr);
	memcpy(&server.sin_addr, he->h_addr,sizeof(struct in_addr));
	if (connect(fd, (struct sockaddr *) &server, sizeof(server)) == -1) {
		perror("connect() error.");
		exit(1);
	}

	struct sockaddr_in localaddr;
	socklen_t len = sizeof(localaddr);
	getsockname(fd, (struct sockaddr*) &localaddr, &len);
	printf("local ip:%s\tport:%d\n", inet_ntoa(localaddr.sin_addr),
			ntohs(localaddr.sin_port));

	struct sockaddr_in peeraddr;
	socklen_t lenp = sizeof(peeraddr);
	getpeername(fd, (struct sockaddr*) &peeraddr, &lenp);
	printf("peer ip:%s\tport:%d\n", inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port));

	close(fd);

}

