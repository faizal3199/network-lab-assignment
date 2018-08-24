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

int stringHexToInt(string x){
	return stoul(x,nullptr,16);
}

string ipToHex(string ip){
	// cout<<ip<<endl;
	string data = "";

	string ret_value = "";

	int i = 0;
	while(i<ip.length()){
		if(ip.at(i) == '.'){
			// cout<<"Got Data:"<<data<<endl;
			ret_value += intToStringHex(stoi(data),2);
			data = "";
			i++;
			continue;
		}
		data.push_back(ip.at(i));
		i++;
	}

	// cout<<"Got Data:"<<data<<endl;
	ret_value += intToStringHex(stoi(data),2);

	return ret_value;
}

int main(){
	// //1 byte
	// int version_header = 0x45;

	// //1 byte
	// int service_type;
	// printf("Type of service(in decimal): ");
	// scanf("%d",&service_type);

	// //2 byte
	// int total_length;
	// printf("Total Length: ");
	// scanf("%d",&total_length);

	// //2 byte
	// int identifier;
	// printf("Identifier: ");
	// scanf("%d",&identifier);

	// //1 bit
	// bool fragment_opt_1;
	// int temp;
	// printf("Don't Fragment[0/1]: ");
	// scanf("%d",&temp);
	// fragment_opt_1 = temp;

	// //1 bit
	// bool fragment_opt_2;
	// printf("More Fragments[0/1]: ");
	// scanf("%d",&temp);
	// fragment_opt_2 = temp;

	// // 14 bits
	// int fragment_offset;
	// printf("Fragment offset (byte offset / 8): ");
	// scanf("%d",&fragment_offset);

	// //1 byte
	// int ttl;
	// printf("TTL: ");
	// scanf("%d",&ttl);

	// //1 byte
	// int protocol;
	// printf("Protocol: ");
	// scanf("%d",&protocol);


	// // cheksum : 2 byte

	// // 4 byte each
	// string source_ip,dest_ip;
	// printf("Source IP(xxx.xxx.xxx.xxx): ");
	// cin>>source_ip;
	// printf("Destination IP(xxx.xxx.xxx.xxx): ");
	// cin>>dest_ip;
	// // cout<<source_ip<<endl;


	string packetInHEx = "";
	cin>>packetInHEx;

	// packetInHEx += intToStringHex(version_header,2);
	// packetInHEx += intToStringHex(service_type,2);
	// packetInHEx += intToStringHex(total_length,4);
	// packetInHEx += intToStringHex(identifier,4);
	// packetInHEx += intToStringHex((((int)(fragment_opt_2))<<13) + (((int)(fragment_opt_1))<<14) + fragment_offset,4);
	// packetInHEx += intToStringHex(ttl,2);
	// packetInHEx += intToStringHex(protocol,2);
	// packetInHEx += intToStringHex(0,4);

	// packetInHEx += ipToHex(source_ip);
	// packetInHEx += ipToHex(dest_ip);



	// for(int in = 0;in<packetInHEx.length();in++){
	// 	cout<<packetInHEx.substr(in,4)<<endl;
	// 	in += 3;
	// }

	//Generate check sum from 2 half bytes
	int checksum_int = 0;
	for(int i=0;i<packetInHEx.length();i+=4){
		checksum_int += stringHexToInt(packetInHEx.substr(i,4));
	}

	cout<<"\n";
	string checksum_hex = intToStringHex(checksum_int,5);

	while(true){
		cout<<"Debug: "<<checksum_hex<<", "<<checksum_int<<endl;
		if(checksum_hex[0] != '0'){
			checksum_int = stringHexToInt(checksum_hex.substr(1,4)) + stringHexToInt(checksum_hex.substr(0,1));
			checksum_hex = intToStringHex(checksum_int,5);
		}
		else{
			checksum_int = stringHexToInt(checksum_hex);
			checksum_hex = intToStringHex(checksum_int,4);
			break;
		}
	}

	checksum_int = checksum_int ^ 0xFFFF;

	cout<<"\nCheckSum is: ";
	cout<<intToStringHex(checksum_int,4)<<endl;

	return 0;
}