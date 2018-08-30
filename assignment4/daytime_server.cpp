#include "networkingAPI.cpp"

string getTimeDataString(){
	time_t time_obj = time(NULL);

	return asctime(localtime(&time_obj));
}

/* UDP: send data on acknowledging data*/
void udp_callback_handler(UDPsocketHandler* socketObj){
	string dataRecv = socketObj->recvData();

	printf("[*] Recivied on UDP: %s\n",&dataRecv[0]);

	socketObj->sendData(getTimeDataString());

	delete socketObj;
}

/* TCP: send data on acknowledging connection and close it*/
void tcp_callback_handler(TCPsocketHandler* socketObj){
	socketObj->sendData(getTimeDataString());

	delete socketObj;
}

int main(int argc,char* argv[]){
	if(argc != 2){
		printf("Usage: %s <port-to-listen>\n",argv[0]);
		exit(1);
	}

	int DAYTIME_PORT = atoi(argv[1]);
	pid_t pid;

	tcpServer tcpServerObj(DAYTIME_PORT);
	udpServer udpServerObj(DAYTIME_PORT);

	pid = fork();
	if(pid < 0){
		printf("[-] Error while forking in main()\n");
		exit(1);
	}

	// As both startListening are blocking new thread is needed
	if(pid == 0){//chlid function
		tcpServerObj.startListening((TCPsocketHandlerCallbackType)&tcp_callback_handler);
	}
	else{
		udpServerObj.startListening((UDPsocketHandlerCallbackType)&udp_callback_handler);
	}

	return 0;
}