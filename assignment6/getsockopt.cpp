#include "../general/networkingAPI.cpp"


//******************
// Explination for setting double value
// https://stackoverflow.com/questions/21146955/getsockopt-so-recvbuf-after-doing-a-set-shows-double-the-value-in-linux
//******************

int doForTCP(int sock_fd){
	//SO_RCVBUF
	int max_size = 0;
	socklen_t max_size_len = sizeof(max_size);

	getsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, (char *)&max_size,&max_size_len);
	printf("TCP default SEND size: %d\n",max_size);


	max_size_len = sizeof(max_size);
	getsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, (char *)&max_size,&max_size_len);
	printf("TCP default RECV size: %d\n",max_size);

	return 0;
}

int doForUDP(int sock_fd){
	//SO_RCVBUF
	int max_size = 0;
	socklen_t max_size_len = sizeof(max_size);

	getsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, (char *)&max_size,&max_size_len);
	printf("UDP default SEND size: %d\n",max_size);


	max_size_len = sizeof(max_size);
	getsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, (char *)&max_size,&max_size_len);
	printf("UDP default RECV size: %d\n",max_size);

	return 0;
}


int setForTCP(int sock_fd){
	//SO_RCVBUF
	int max_size = 26384;
	socklen_t max_size_len = sizeof(max_size);

	if(setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, (char *)&max_size,max_size_len)==-1){
		perror("Error occured ");
	}
	printf("TCP default changed to SEND size: %d\n",max_size);

	max_size = 107380;
	max_size_len = sizeof(max_size);
	
	if(setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, (char *)&max_size,max_size_len)==-1){
		perror("Error occured ");
	}
	
	printf("TCP default changed to RECV size: %d\n",max_size);

	return 0;
}

int setForUDP(int sock_fd){
	//SO_RCVBUF
	int max_size = 112992;
	socklen_t max_size_len = sizeof(max_size);

	setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, (char *)&max_size,max_size_len);
	printf("UDP default changed to SEND size: %d\n",max_size);


	max_size = 200992;
	max_size_len = sizeof(max_size);
	setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, (char *)&max_size,max_size_len);
	printf("UDP default changed to RECV size: %d\n",max_size);

	return 0;
}

int main(){
	tcpServer* TCPsockObj= new tcpServer(8085);
	int tcp_sock_fd = TCPsockObj->getSocketFD();

	doForTCP(tcp_sock_fd);

	udpServer* UDPsockObj= new udpServer(8085);
	int udp_sock_fd = UDPsockObj->getSocketFD();

	doForUDP(udp_sock_fd);

	printf("\n");

	setForTCP(tcp_sock_fd);
	setForUDP(udp_sock_fd);

	printf("\n");

	doForTCP(tcp_sock_fd);
	doForUDP(udp_sock_fd);

	delete TCPsockObj;
	delete UDPsockObj;

	return 0;
}