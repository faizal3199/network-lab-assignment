#include "networkingAPI.cpp"

void trim_crlf(string &data){
	while(true)
		if(data.back() == '\n' || data.back() == '\r' || data.back() == '\x00')
			data.pop_back();
		else
			return;
}

void server_listener(TCPsocketHandler* socketObj){
	string recved;

	while(true){
		recved = socketObj->recvData();
		
		if(recved.length() == 0){
			delete socketObj;
			return;
		}

		trim_crlf(recved);

		printf("\r%s\n[YOU] ",&recved[0]);
		fflush(stdout);
	}
}

void socket_handler(TCPsocketHandler* socketObj){
	thread	new_thread(server_listener,socketObj);
	new_thread.detach();

	string input;
	char inputBuffer[200];
	while(true){
		printf("\r[YOU] ");
		fflush(stdout);

		cin.getline(&inputBuffer[0],200);
		input = inputBuffer;
		trim_crlf(input);

		socketObj->sendData(input);
		// printf("\n");
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