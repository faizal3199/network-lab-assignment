class tcpServer{
private:
	int serverSockFD, clientConnectionSockFD;
	socklen_t clientConnectionLength;

	sockaddr_in sock_server,client_connection;

public:
	tcpServer(int port_number,int max_connection=50){
		this->serverSockFD =  socket(AF_INET, SOCK_STREAM, 0);

		if(this->serverSockFD < 0){
			error("[-] {TCP} Error creating FD for new server socket");
		}

		printf("[+] {TCP} Server socket created\n");
		// Empty out the memory
		bzero((char *) &this->sock_server, sizeof(this->sock_server));

		this->sock_server.sin_family = AF_INET;
		this->sock_server.sin_addr.s_addr = INADDR_ANY;
		this->sock_server.sin_port = htons(port_number);

		if(bind(this->serverSockFD,(sockaddr*)&this->sock_server,sizeof(this->sock_server)) < 0){
			error("[-] {TCP} Error binding socket to FD");
		}
		printf("[+] {TCP} Socket binded to FD\n");

		printf("[+] {TCP} Socket open for connections\n");
		listen(this->serverSockFD,max_connection);
	}

	~tcpServer(){
		printf("[+] {TCP} Closing server socket. Freeing FD\n");
		closeConnection();
	}

	int getSocketFD(){
		return this->serverSockFD;
	}

	void closeConnection(){
		close(this->serverSockFD);
	}

	//Blocking function
	void startListening(TCPsocketHandlerCallbackType callback_function){//Must pass a callback function
		clientConnectionLength = sizeof(client_connection);

		printf("[+] {TCP} Server waiting for connections\n");

		while(true){
			/*Blocking function*/
			clientConnectionSockFD = accept(this->serverSockFD,(sockaddr *) &this->client_connection, &clientConnectionLength);

			if(clientConnectionSockFD < 0){
				error("[-] {TCP} Error invalid FD for new connection");
			}

			printf("[+] {TCP} Recivied connection from %s : %d\n",inet_ntoa(client_connection.sin_addr), ntohs(client_connection.sin_port));

			
			printf("[+] {TCP} New thread created. Callback handler called\n");
			thread temp_thread(*callback_function,new TCPsocketHandler(client_connection,clientConnectionSockFD));
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
