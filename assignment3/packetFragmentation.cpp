#include<bits/stdc++.h>
using namespace std;

string intToStringHex(int x,int fill = 4){
	stringstream s;
	s << setfill('0') << setw(fill) << hex << x;
	string temp = s.str();
	temp = temp.substr(temp.length()-fill,fill);
	transform(temp.begin(), temp.end(),temp.begin(),::toupper);
	return temp;
}

string readFromFile(ifstream& inputFile){
	stringstream output;
	char temp;

	while(inputFile.read(&temp,(uint32_t)1)){
		// cout<<(int)temp;
		output<<setfill('0')<< setw(2)<<hex<<(int)temp;
	}
	return output.str();
}

void fragment(int mtu,string& packetData){
	// All in bytes;
	int nfb = (mtu - 20)/8;

	int max_data_packet_len = nfb * 8;// Max length of data packet

	int ip_packet_len;

	int data_length = packetData.length()/2;

	int curr_data_len;
	bool DF=false,MF=false;
	
	if(data_length <= max_data_packet_len) DF=true;

	int fragment_block_count = 0;

	int index_of_packetData = 0;
	while(data_length != 0){
		// cout<<data_length<<endl;
		if(data_length > max_data_packet_len){
			curr_data_len = max_data_packet_len;
			data_length -= curr_data_len;
			MF = true;
		}
		else{
			MF = false;
			curr_data_len = data_length;
			data_length -= curr_data_len;
		}

		ip_packet_len = 20 + curr_data_len;
		// hex
		cout<<"0000";
		cout<<intToStringHex(ip_packet_len,4);
		cout<<"0000";
		cout<<intToStringHex((((int)(MF))<<13) + (((int)(DF))<<14) + fragment_block_count,4);
		cout<<"00000000";
		cout<<"00000000";
		cout<<"00000000";
		cout<<packetData.substr(index_of_packetData,curr_data_len*2);
		cout<<endl;
		index_of_packetData += 2*curr_data_len;

		fragment_block_count += curr_data_len/8;
	}
}

int main(int argc,char** argv){

	if(argc != 3){
		printf("Usage: %s <MTU> <file location>\n",argv[0]);
		return 1;
	}

	int MTU;
	MTU = stoi(argv[1]);

	ifstream inputFile;
	inputFile.open(argv[2]);

	string fileData = readFromFile(inputFile);
	// cout<<fileData<<endl;
	inputFile.close();

	fragment(MTU,fileData);
	
	return 0;
}