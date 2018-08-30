#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

class TCPsocketHandler{
private:
	sockaddr_in origin_connection;
	int socket_fd;
	
public:
	TCPsocketHandler(sockaddr_in origin_connection,int fd){
		this->socket_fd = fd;
		this->origin_connection = origin_connection;
	}

	~TCPsocketHandler(){
		printf("[+] Freed connection socket\n");
		closeSocket();
	}

	string recvData(){
		int status_code;
		string return_value;

		char readBuffer[1024];

		while(true){
			bzero(readBuffer,1024);

			status_code = read(this->socket_fd,readBuffer,1023);

			if(status_code < 0){
				error("[-] Error reading from socket connection");
			}

			string temp(readBuffer);

			return_value += temp;

			if(status_code < 1023){//Maybe data is pending
				break;
			}
		}

		return return_value;
	}

	void sendData(string data){
		int status_code = write(this->socket_fd, &data[0],data.length());
		if(status_code<0){
			error("[-] Error sending to socket connection");
		}
		else if(status_code < data.length()){
			printf("[*] Error writing full data. Only partial data written\n");
		}
	}

	void closeSocket(){
		close(this->socket_fd);
	}

private:
	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
};
using TCPsocketHandlerCallbackType = void(*)(TCPsocketHandler*);


class tcpServer{
private:
	int serverSockFD, clientConnectionSockFD;
	socklen_t clientConnectionLength;

	sockaddr_in sock_server,client_connection;

public:
	tcpServer(int port_number,int max_connection=50){
		this->serverSockFD =  socket(AF_INET, SOCK_STREAM, 0);

		if(this->serverSockFD < 0){
			error("[-] Error creating socket");
		}
		printf("[+] Socket created\n");
		// Empty out the memory
		bzero((char *) &this->sock_server, sizeof(this->sock_server));

		this->sock_server.sin_family = AF_INET;
		this->sock_server.sin_addr.s_addr = INADDR_ANY;
		this->sock_server.sin_port = htons(port_number);

		if(bind(this->serverSockFD,(sockaddr*)&this->sock_server,sizeof(this->sock_server)) < 0){
			error("[-] Error binding socket to filedescriptor");
		}
		printf("[+] Socket binded to FD\n");

		listen(this->serverSockFD,max_connection);
	}

	~tcpServer(){
		printf("[+] Sever stopped accepting connections\n");
		closeConnection();
	}

	void closeConnection(){
		close(this->serverSockFD);
	}
	//BLocking function
	void startListening(TCPsocketHandlerCallbackType callback_function){//Must pass a callback function
		clientConnectionLength = sizeof(client_connection);

		pid_t pid;

		printf("[+] Server waiting for connections\n");

		while(true){
			/*Blocking function*/
			clientConnectionSockFD = accept(this->serverSockFD,(sockaddr *) &this->client_connection, &clientConnectionLength);

			if(clientConnectionSockFD < 0){
				error("[-] Error invalid FD for new connection");
			}

			printf("[+] Recivied connection from %s : %d\n",inet_ntoa(client_connection.sin_addr), ntohs(client_connection.sin_port));

			pid = fork();

			if(pid < 0){
				error("[-] Error while forking process");
			}
			else if(pid == 0){//child call the callbak here
				printf("[+] Callback handler called\n");
				(*callback_function)(new TCPsocketHandler(client_connection,clientConnectionSockFD));
				exit(0);
			}
		}

	}

private:
	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
};

class tcpClient{
private:
	int socketFD;
	sockaddr_in server_conn;

public:
	tcpClient(string hostname,int port_number){
		hostent *resolved_address;

		this->socketFD = socket(AF_INET, SOCK_STREAM, 0);

		if(this->socketFD < 0){
			error("[-] Error creating socket");
		}

		resolved_address = gethostbyname(&hostname[0]);

		if(resolved_address == NULL){
			error("[-] Can't resolve host address");
		}

		bzero((char *)&this->server_conn,sizeof(this->server_conn));

		this->server_conn.sin_family = AF_INET;
		this->server_conn.sin_port = htons(port_number);

		bcopy((char*)resolved_address->h_addr, (char*)&this->server_conn.sin_addr.s_addr, resolved_address->h_length);

		if(connect(this->socketFD,(sockaddr *)&this->server_conn,sizeof(this->server_conn)) < 0){
			error("[-] Error conencting to server");
		}
		printf("[+] Connection to server established\n");
		// FD is already attached still use a anoyher function to call callback function
	}

	~tcpClient(){
		//Actually handled by TCPsocketHandler but still let's do it
		printf("[+] Connection to server closed\n");
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

class UDPsocketHandler{
private:
	int socketFD;
	sockaddr_in conn_address;
	string data;
	bool readAllowed;

public:
	UDPsocketHandler(sockaddr_in conn_address,int socketFD,string data,bool readAllowed=false){
		this->conn_address = conn_address;
		this->socketFD = socketFD;
		this->data = data;
		this->readAllowed = readAllowed;
	}

	string recvData(){
		if(this->readAllowed){//Update the data
			int datagram_size;
			char bufferRead[2049];
			bzero(bufferRead,2049);

			socklen_t client_connection_length = sizeof(this->conn_address);
			datagram_size = recvfrom(this->socketFD,bufferRead,2048,MSG_WAITALL,(sockaddr *)&this->conn_address,&client_connection_length);

			if(datagram_size < 0){
				error("[-] Error reading from socket");
			}

			string temp(bufferRead);
			this->data = temp;
		}

		return this->data;
	}

	void sendData(string data_to_send){
		int status_code = sendto(this->socketFD,&data_to_send[0],data_to_send.size(),MSG_CONFIRM | MSG_DONTWAIT,(sockaddr *)&this->conn_address,sizeof(this->conn_address));

		if(status_code < 0){
			error("[-] Error sending data over socket");
		}
		else if(status_code < data_to_send.length()){
			printf("[*] Only partial data written.\n");
		}
	}

	void closeSocket(){
		close(this->socketFD);
	}

private:
	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
};
using UDPsocketHandlerCallbackType = void(*)(UDPsocketHandler*);


class udpServer{
private:
	sockaddr_in server_address, client_address;
	int socketFD;

public:
	udpServer(int port_number){
		this->socketFD = socket(AF_INET, SOCK_DGRAM, 0);

		if(this->socketFD < 0){
			error("[-] Error creating socket");
		}

		bzero((char *)&server_address,sizeof(server_address));

		this->server_address.sin_family = AF_INET;
		this->server_address.sin_port = ntohs(port_number);
		this->server_address.sin_addr.s_addr = INADDR_ANY;

		if(bind(this->socketFD,(sockaddr *)&this->server_address,sizeof(this->server_address)) < 0){
			error("[-] Error binding socket");
		}

		printf("[+] Socket open for connections\n");
	}

	~udpServer(){
		printf("[+] Closing connection. Freeing FD\n");
		closeSocket();
	}

	void closeSocket(){
		close(this->socketFD);
	}

	void startListening(UDPsocketHandlerCallbackType callback_function){
		printf("[+] Server waiting for connections\n");

		char bufferRead[2049]; //read buffer as recvfrom puts data into buffer
		int datagram_size;

		socklen_t client_connection_length = sizeof(this->client_address);
		pid_t pid;

		while(true){
			bzero(bufferRead,2049);
			bzero((char*)&client_address,sizeof(client_address));

			/*returns the length of the message on successful completion*/
			datagram_size = recvfrom(this->socketFD,bufferRead,2048,0,(sockaddr *)&this->client_address,&client_connection_length);

			if(datagram_size < 0){
				error("[-] Error reading from socket");
			}

			printf("[+] Recivied connection from %s : %d\n",inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

			pid = fork();

			if(pid < 0 ){
				error("[-] Error creating new thread");
			}
			else if(pid == 0){//Child process. Handle callback here
				string temp(bufferRead);
				(*callback_function)(new UDPsocketHandler(this->client_address,this->socketFD,temp));
				exit(0);
			}
		}
	}

private:
	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
};

class udpClient{
private:
	sockaddr_in server_address;
	int socketFD;

public:
	udpClient(string hostname,int port_number){
		hostent *resolved_address;

		this->socketFD = socket(AF_INET,SOCK_DGRAM,0);

		if(this->socketFD < 0){
			error("[-] Error creating socket");
		}

		resolved_address = gethostbyname(&hostname[0]);

		if(resolved_address == NULL){
			error("[-] Can't resolve host address");
		}

		bzero((char*)&server_address,sizeof(server_address));

		bcopy((char*)resolved_address->h_addr, (char*)&this->server_address.sin_addr.s_addr, resolved_address->h_length);
		this->server_address.sin_family = AF_INET;
		this->server_address.sin_port = htons(port_number);

		printf("[+] Ready to connect to server\n");
	}

	void startTalking(UDPsocketHandlerCallbackType callback_function){
		(*callback_function)(new UDPsocketHandler(this->server_address,this->socketFD,"",true));
	}

	~udpClient(){
		printf("[+] Closing connection to server\n");
		closeSocket();
	}

	void closeSocket(){
		close(this->socketFD);
	}

private:
	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
};

// void callback_function(UDPsocketHandler* socketObj){

/* UDP client should not accept the message from server before acknoledging itself*/