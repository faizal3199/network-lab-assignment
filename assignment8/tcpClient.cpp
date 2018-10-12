#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

#include "customExceptions.cpp"
#include "TCPsocketHandler.cpp"

class tcpClient{
private:
	int socketFD;
	sockaddr_in server_conn;

public:
	tcpClient(string hostname,int port_number){
		hostent *resolved_address;

		this->socketFD = socket(AF_INET, SOCK_STREAM, 0);

		if(this->socketFD < 0){
			error("[-] {TCP} Error creating FD for new server socket");
		}

		resolved_address = gethostbyname(&hostname[0]);

		if(resolved_address == NULL){
			error("[-] {TCP} Can't resolve host address");
		}

		bzero((char *)&this->server_conn,sizeof(this->server_conn));

		this->server_conn.sin_family = AF_INET;
		this->server_conn.sin_port = htons(port_number);

		bcopy((char*)resolved_address->h_addr, (char*)&this->server_conn.sin_addr.s_addr, resolved_address->h_length);

		if(connect(this->socketFD,(sockaddr *)&this->server_conn,sizeof(this->server_conn)) < 0){
			error("[-] {TCP} Error connecting to server.");
		}

		printf("[+] {TCP} Connection to server established. Waiting for callback function.\n");
		// FD is already attached still use a another function to call callback function
	}

	~tcpClient(){
		//Actually handled by TCPsocketHandler but still let's do it
		printf("[+] {TCP} Closing connection to server. Freeing FD\n");
		closeSocket();
	}

	void closeSocket(){
		close(this->socketFD);
	}

	//Call calback function
	void startTalking(TCPsocketHandlerCallbackType callback_function){
		(*callback_function)(new TCPsocketHandler(this->server_conn,this->socketFD));
	}

private:
	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
};