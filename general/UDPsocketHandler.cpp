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
				error("[-] {UDP} Error reading from socket connection");
			}

			string temp(begin(bufferRead),begin(bufferRead)+status_code);
			this->data = temp;
		}

		if(this->data.length()==0)
			throw zeroLengthException();

		return this->data;
	}

	void sendData(string data_to_send){
		int status_code = sendto(this->socketFD,&data_to_send[0],data_to_send.size(),MSG_CONFIRM | MSG_DONTWAIT,(sockaddr *)&this->conn_address,sizeof(this->conn_address));

		if(status_code < 0){
			error("[-] {UDP} Error sending data on socket connection");
		}
		else if(status_code < data_to_send.length()){
			printf("[*] {UDP} Error writing full data. Only partial data written\n");
		}
	}

	string getIPAddress(){
		return string(inet_ntoa(this->conn_address.sin_addr));
	}

	int getPortNumber(){
		return ntohs(this->conn_address.sin_port);
	}
	
	int getSocketFD(){
		return this->socketFD;
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