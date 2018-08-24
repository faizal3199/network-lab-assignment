#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

using namespace std;

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void toUpperCase(char* arr){
	unsigned char *p = (unsigned char *)arr;

	while(*p){
		*p = toupper((unsigned char)*p);
		p++;
	}
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;

	socklen_t clilen;

	char readBuffer[1024];

	struct sockaddr_in serv_addr, cli_addr;

	int n;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	sockfd =  socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) 
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;  

	serv_addr.sin_addr.s_addr = INADDR_ANY;  

	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR on binding");

	listen(sockfd,50);

	clilen = sizeof(cli_addr);

	while(true){
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");

		printf("server: got connection from %s port %d\n",inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

		bzero(readBuffer,1024);

		n = read(newsockfd,readBuffer,1023);
		if (n < 0){
			error("ERROR reading from socket");
		}

		printf("Recievied message: %s\n",readBuffer);

		toUpperCase(readBuffer);

		send(newsockfd, readBuffer,strlen(readBuffer), 0);

		close(newsockfd);

	}
	close(sockfd);
	
	return 0;
}