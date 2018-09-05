class udpServer{
private:
	sockaddr_in server_address, client_address;
	int socketFD;

public:
	udpServer(int port_number){
		this->socketFD = socket(AF_INET, SOCK_DGRAM, 0);

		if(this->socketFD < 0){
			error("[-] {UDP} Error creating FD for new server socket");
		}

		printf("[+] {UDP} Server socket created\n");
		bzero((char *)&server_address,sizeof(server_address));

		this->server_address.sin_family = AF_INET;
		this->server_address.sin_port = ntohs(port_number);
		this->server_address.sin_addr.s_addr = INADDR_ANY;

		if(bind(this->socketFD,(sockaddr *)&this->server_address,sizeof(this->server_address)) < 0){
			error("[-] {UDP} Error binding socket to FD");
		}
		printf("[+] {UDP} Socket binded to FD\n");

		printf("[+] {UDP} Socket open for connections\n");
	}

	~udpServer(){
		printf("[+] {UDP} Closing server socket. Freeing FD\n");
		closeSocket();
	}

	void closeSocket(){
		close(this->socketFD);
	}

	void startListening(UDPsocketHandlerCallbackType callback_function){
		char bufferRead[2049]; //read buffer as recvfrom puts data into buffer
		int datagram_size;

		socklen_t client_connection_length = sizeof(this->client_address);
		
		printf("[+] {UDP} Server waiting for connections\n");

		while(true){
			bzero(bufferRead,2049);
			bzero((char*)&client_address,sizeof(client_address));

			/*returns the length of the message on successful completion*/
			datagram_size = recvfrom(this->socketFD,bufferRead,2048,0,(sockaddr *)&this->client_address,&client_connection_length);

			if(datagram_size < 0){
				error("[-] {UDP} Error data from new connection");
			}

			printf("[+] {UDP} Recivied connection from %s : %d\n",inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

			string temp(bufferRead);
			printf("[+] {UDP} New thread created. Callback handler called\n");
			thread temp_thread(*callback_function,new UDPsocketHandler(this->client_address,this->socketFD,temp));
			temp_thread.detach();
		}
	}

private:
	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
};