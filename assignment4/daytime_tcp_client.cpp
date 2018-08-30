#include "networkingAPI.cpp"

void callback_handler(TCPsocketHandler* socketObj){
	cout<<socketObj->recvData();

	delete socketObj;
}

int main(int argc,char* argv[]){
	if(argc != 3){
		printf("Usage: %s <host-to-connect> <port-to-listen>\n",argv[0]);
		exit(1);
	}

	tcpClient clientObj(string(argv[1]),atoi(argv[2]));

	clientObj.startTalking((TCPsocketHandlerCallbackType)&callback_handler);
}