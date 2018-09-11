#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;
int main(int argc, char const *argv[]){

	if (argc != 2){
		printf("Usage: %s <ip-address>\n",argv[0]);
		return 1;
	}

	sockaddr_in address;

	bzero((char*)&address,sizeof(address));

	address.sin_family = AF_INET;
	address.sin_port = 0;

	if(inet_pton(AF_INET,argv[1],&address.sin_addr) < 0){
		perror("[-] Error while converting to network byte order");
		exit(1);
	}

	char host[2048];

	bzero(host,sizeof(host));
	
	int status_code = getnameinfo(
		(sockaddr *)&address, (socklen_t) sizeof(address),
		host, (socklen_t)sizeof(host),
		NULL, 0,
		NI_NAMEREQD);

	char* message = (char *)"";
	switch(status_code){
		case EAI_AGAIN:
			message = (char *)"The name could not be resolved at this time. Try again later.";
			break;
		case EAI_BADFLAGS:
			message = (char *)"The flags argument has an invalid value.";
			break;
		case EAI_FAIL:
			message = (char *)"A nonrecoverable error occurred.";
			break;
		case EAI_FAMILY:
			message = (char *)"The address family was not recognized, or the address length was invalid for the specified family.";
			break;
		case EAI_MEMORY:
			message = (char *)"Out of memory.";
			break;
		case EAI_NONAME:
			message = (char *)"The name does not resolve for the supplied arguments.";
			break;
		case EAI_OVERFLOW:
			message = (char *)"The buffer pointed to by host or serv was too small.";
			break;
		case EAI_SYSTEM:
			message = (char *)"A system error occurred.  The error code can be found in errno.";
			break;
	};

	if(strlen(message) != 0){
		perror(message);
		exit(1);
	}

	printf("PTR record for %s => %s\n",argv[1],host);

	return 0;
}