class udpClient{
private:
	sockaddr_in server_address;
	int socketFD;

public:
	udpClient(string hostname,int port_number){
		hostent *resolved_address;

		this->socketFD = socket(AF_INET,SOCK_DGRAM,0);

		if(this->socketFD < 0){
			error("[-] {UDP} Error creating FD for new server socket");
		}

		resolved_address = gethostbyname(&hostname[0]);

		if(resolved_address == NULL){
			error("[-] {UDP} Can't resolve host address");
		}

		bzero((char*)&server_address,sizeof(server_address));

		bcopy((char*)resolved_address->h_addr, (char*)&this->server_address.sin_addr.s_addr, resolved_address->h_length);
		this->server_address.sin_family = AF_INET;
		this->server_address.sin_port = htons(port_number);

		printf("[+] {UDP} Connection created to server. Waiting for callback function.\n");
	}

	~udpClient(){
		printf("[+] {UDP} Closing connection to server. Freeing FD\n");
		closeSocket();
	}

	void closeSocket(){
		close(this->socketFD);
	}

	void startTalking(UDPsocketHandlerCallbackType callback_function){
		(*callback_function)(new UDPsocketHandler(this->server_address,this->socketFD,"",true));
	}

private:
	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
};
// void callback_function(UDPsocketHandler* socketObj)
/* UDP client should not accept the message from server before acknoledging itself*/