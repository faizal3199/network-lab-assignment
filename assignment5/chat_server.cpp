#include "networkingAPI.cpp"

/*--------Server-----------*/
// Ask for group on joining
// or create new group
// 
// relay messages to each other

struct chatRoomStruct{
	string room_id;
	int max_size;
	vector<TCPsocketHandler*> members;

	chatRoomStruct(int max_size,string chat_room_id){
		this->room_id = chat_room_id;
		this->max_size = max_size;
		this->members.clear();
	}

	void blocking_listen_function(TCPsocketHandler* newMember){
		string tempRecv;
		while(true){
			tempRecv = newMember->recvData();
			this->relayMessage(tempRecv,newMember);
		}
	}

	void add_new_member(TCPsocketHandler* newMember){
		if(this->members.size() == max_size){
			newMember->sendData("Group size is max can't connect. Clossing connection");
			delete newMember;
			return;
		}
		this->members.push_back(newMember);
		this->relayMessage("------ New member added ------\n",newMember);
		this->blocking_listen_function(newMember);
	}

	void create_new_thread(TCPsocketHandler* newMember){
		pid_t pid;
		pid = fork();

	}

	void relayMessage(string message,TCPsocketHandler* sender){
		for(int i=0;i<this->members.size();i++){
			if(sender != this->members[i]){
				this->members[i]->sendData(message);
			}
		}
	}
};

map<string,chatRoomStruct*> chatRoomMapping;

void new_client_hanlder(TCPsocketHandler* socketObj){
	printf("[*] NEW client connected\n");
	socketObj->sendData("Join or Create new room:[J/C]");

	string response = socketObj->recvData();
	printf("[*] Client response: %s\n",&response[0]);

	response = response.substr(0,1); //Strip CRLF

	if(response == "C" || response == "c"){
		socketObj->sendData("Enter chat room ID to create:");
		string chat_room_id = socketObj->recvData();

		socketObj->sendData("Enter max size of chat room:");
		int max_size_chat_room = stoi(socketObj->recvData());

		string temp = "Creating new chat room of ID "+chat_room_id + " with max size " + to_string(max_size_chat_room); 
		socketObj->sendData(temp);

		chatRoomMapping.insert({chat_room_id,new chatRoomStruct(max_size_chat_room,chat_room_id)});
		chatRoomMapping[chat_room_id]->add_new_member(socketObj);
	}
	else if(response == "J" || response == "j"){
		socketObj->sendData("Enter chat room ID to join:");
		string chat_room_id = socketObj->recvData();

		if(chatRoomMapping.count(chat_room_id) == 1){
			chatRoomMapping[chat_room_id]->add_new_member(socketObj);
		}
		else{
			socketObj->sendData("No such room exists. Closing connection.\n");
			delete socketObj;
			return;
		}
	}
	else{
		socketObj->sendData("Invalid response. Closing connection.\n");
		delete socketObj;
		return;
	}
}

int main(int argc,char* argv[]){
	if(argc != 2){
		printf("Usage: %s <port-to-start>\n",argv[0]);
		exit(1);
	}

	tcpServer chatServer(atoi(argv[1]),100);

	chatServer.startListening((TCPsocketHandlerCallbackType)&new_client_hanlder);
}