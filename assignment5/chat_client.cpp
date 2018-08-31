#include "networkingAPI.cpp"

void server_listener(TCPsocketHandler* socketObj){
	string recved;
	while(true){
		recved = socketObj->recvData();
		printf("\r[SERVER] %s",&recved[0]);
	}
}

void socket_handler(TCPsocketHandler* socketObj){
	pid_t pid = fork();
	if(pid == 0){//Make child process listen to server
		server_listener(socketObj);
	}

	string input;
	while(true){
		printf("[YOU] ");
		cin>>input;
		socketObj->sendData(input);
		printf("\n");
	}
}

int main(int argc,char* argv[]){
	if(argc != 3){
		printf("Usage: %s <chat-server> <port>\n",argv[0]);
		exit(1);
	}

	tcpClient clientObj(string(argv[1]),atoi(argv[2]));
	clientObj.startTalking((TCPsocketHandlerCallbackType)&socket_handler);
}