#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

using namespace std;

int main(int argc,char** argv){
	// string ip  = (argv[1]);

	// sockaddr_in addr;

	// addr.sin_family = AF_INET;
	// addr.sin_port = htons(0);
	// addr.sin_addr.s_addr = inet_aton(&ip[0]);

	in_addr addr;
	inet_aton(argv[1],&addr);

	hostent* solved = gethostbyaddr((in_addr*)&addr,(socklen_t)(sizeof(addr)),AF_INET);
	if(solved == NULL){
		perror("Not resolved");
		return 1;
	}

	if(solved->h_name){
		cout<<solved->h_name<<endl;
	}
	else{
		cout<<"Error"<<endl;
	}

	return 0;
}