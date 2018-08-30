#include "networkingAPI.cpp"


string data_to_send;
void callback_handler(TCPsocketHandler* socketObj){

	printf("[*] Sent: %s\n",&data_to_send[0]);
	
	socketObj->sendData(data_to_send);

	cout<<socketObj->recvData()<<endl;

	delete socketObj;
}

int main(int argc,char* argv[]){
	if(argc != 4){
		printf("Usage: %s <host-to-connect> <port-to-listen> <data-to-change>\n",argv[0]);
		exit(1);
	}

	data_to_send = argv[3];

	tcpClient clientObj(string(argv[1]),atoi(argv[2]));

	clientObj.startTalking((TCPsocketHandlerCallbackType)&callback_handler);
}