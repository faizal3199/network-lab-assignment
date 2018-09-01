#include "networkingAPI.cpp"

void trim_crlf(string &data){
	while(true)
		if(data.back() == '\n' || data.back() == '\r' || data.back() == '\x00')
			data.pop_back();
		else
			return;
}

struct chatRoomStruct{
	string chat_room_id;
	int max_size;
	map<string,TCPsocketHandler*> members;

	chatRoomStruct(int max_size,string chat_room_id){
		this->chat_room_id = chat_room_id;
		this->max_size = max_size;
		this->members.clear();
	}

	void remove_member(string username){
		string temp = "["+this->chat_room_id+"] "+username+" left the chat room";
		this->relayMessage(temp,username);

		delete this->members[username];
		
		this->members.erase(this->members.find(username));

		return;
	}

	void blocking_listen_function(string username){
		TCPsocketHandler* newMember = members[username];
		string tempRecv;
		while(true){
			tempRecv = newMember->recvData();
			
			if(tempRecv.size() == 0){
				remove_member(username);
				return;
			}

			trim_crlf(tempRecv);

			tempRecv = "["+username+"] " + tempRecv;

			this->relayMessage(tempRecv,username);
		}
	}

	void add_new_member(TCPsocketHandler* newMember){
		if(this->members.size() == this->max_size){
			newMember->sendData("[SERVER] Chat room is full. Can't connect. Closing connection.\n");
			delete newMember;
			return;
		}

		string temp = "[SERVER] Joined the chat room [" + this->chat_room_id + "]";
		newMember->sendData(temp);
		string username;

		while(true){
			temp = "[SERVER] Provide your username:";
			newMember->sendData(temp);

			username = newMember->recvData();
			if(username.length() == 0){
				delete newMember;
				return;
			}

			if(members.count(username) == 1 || username == this->chat_room_id){
				temp = "[SERVER] Username already exists in the group try another.";
				newMember->sendData(temp);
			}
			else{
				members.insert({username,newMember});
				temp = "["+this->chat_room_id+"] Welcome, "+username;
				newMember->sendData(temp);
				break;
			}
		}

		temp = "["+this->chat_room_id+"] "+username+" joined the chat room.";
		this->relayMessage(temp,username);
		this->blocking_listen_function(username);
	}

	void relayMessage(string message,string username){
		for(auto it: members){
			if(it.first != username){
				it.second->sendData(message);
			}
		}
	}
};

map<string,chatRoomStruct*> chatRoomMapping;

void new_client_hanlder(TCPsocketHandler* socketObj){
	printf("[*] NEW client connected\n");
	socketObj->sendData("[SERVER] [J]oin or [C]reate new room ?[J/C]\n");

	string response = socketObj->recvData();
	if(response.length() == 0){
		delete socketObj;
		return;
	}

	printf("[*] Client response: %s\n",&response[0]);

	response = response.substr(0,1); //Strip CRLF

	string temp;

	if(response == "C" || response == "c"){
		socketObj->sendData("[SERVER] Enter chat room ID to create:\n");
		string chat_room_id = socketObj->recvData();

		if(chat_room_id.length() == 0){
			delete socketObj;
			return;
		}

		if(chatRoomMapping.count(chat_room_id) == 1){
			temp = "[SERVER] Chat room with id " + chat_room_id + " already exists.\n";
			socketObj->sendData(temp);
			delete socketObj;
			return;
		}

		socketObj->sendData("[SERVER] Enter max size of chat room:\n");
		int max_size_chat_room = stoi(socketObj->recvData());

		temp = "[SERVER] Created new chat room ["+chat_room_id + "] . Maximum members allowed: " + to_string(max_size_chat_room) + "\n"; 
		socketObj->sendData(temp);

		chatRoomMapping.insert({chat_room_id,new chatRoomStruct(max_size_chat_room,chat_room_id)});
		chatRoomMapping[chat_room_id]->add_new_member(socketObj);
	}
	else if(response == "J" || response == "j"){
		socketObj->sendData("[SERVER] Enter chat room ID to join:\n");
		string chat_room_id = socketObj->recvData();

		if(chat_room_id.length() == 0){
			delete socketObj;
			return;
		}

		if(chatRoomMapping.count(chat_room_id) == 1){
			chatRoomMapping[chat_room_id]->add_new_member(socketObj);
		}
		else{
			socketObj->sendData("[SERVER] No such room exists. Closing connection.\n");
			delete socketObj;
			return;
		}
	}
	else{
		socketObj->sendData("[SERVER] Invalid response. Closing connection.\n");
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