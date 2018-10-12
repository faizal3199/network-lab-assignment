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
		printf("[+] {TCP} Closing connection socket. Freeing FD\n");
		closeSocket();
	}

	string recvData(){
		int status_code;
		string return_value;

		char readBuffer[1025];

		while(true){
			bzero(readBuffer,1025);

			status_code = read(this->socket_fd,readBuffer,1024);

			if(status_code < 0){
				error("[-] {TCP} Error reading from socket connection");
			}

			string temp(begin(readBuffer),begin(readBuffer)+status_code);

			return_value += temp;

			if(status_code == 0){//Nothing to read exit now
				break;
			}
		}

		if(return_value.length()==0)
			throw zeroLengthException();

		return return_value;
	}

	void sendData(string data){
		int status_code = write(this->socket_fd, &data[0],data.length());
		if(status_code<0){
			error("[-] {TCP} Error sending data on socket connection");
		}
		else if(status_code < data.length()){
			printf("[*] {TCP} Error writing full data. Only partial data written\n");
		}
	}

	string getIPAddress(){
		return string(inet_ntoa(this->origin_connection.sin_addr));
	}

	int getPortNumber(){
		return ntohs(this->origin_connection.sin_port);
	}

	int getSocketFD(){
		return this->socket_fd;
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