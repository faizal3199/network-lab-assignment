#include "networkingAPI.cpp"

void callback_handler(UDPsocketHandler* socketObj){
	socketObj->sendData("Give Time");

	cout<<socketObj->recvData();

	socketObj.closeSocket();
	delete socketObj;
}

int main(int argc,char* argv[]){
	if(argc != 3){
		printf("Usage: %s <host-to-connect> <port-to-listen> <data-to-change>\n",argv[0]);
		exit(1);
	}

	udpClient clientObj(string(argv[1]),atoi(argv[2]));

	clientObj.startTalking((UDPsocketHandlerCallbackType)&callback_handler);
}