#include "networkingAPI.cpp"

void transform_to_uppercase(string &temp){
	transform(temp.begin(), temp.end(),temp.begin(),::toupper);
}

void callback_handler(TCPsocketHandler* socketObj){
	string data_from_client = socketObj->recvData();

	printf("[*] Recivied: %s\n",&data_from_client[0]);
	transform_to_uppercase(data_from_client);
	
	socketObj->sendData(data_from_client);

	delete socketObj;
}

int main(int argc,char* argv[]){
	if(argc != 2){
		printf("Usage: %s <port-to-listen>\n",argv[0]);
		exit(1);
	}

	tcpServer serverObj(atoi(argv[1]));
	serverObj.startListening((TCPsocketHandlerCallbackType)&callback_handler);
}