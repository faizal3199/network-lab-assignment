#include<bits/stdc++.h>
using namespace std;

int stringHexToInt(string x){
  return stoul(x,nullptr,16);
}

int charPairToInt(char a,char b){
	string z ="";
	z.push_back(a);
	z.push_back(b);
	return stringHexToInt(z);
}

int main(){
	string ipHeaderLen;
	string fragmentBlockNum;
	string currData;

	map<int,string> dataPackets;

	int data_len = 0;

	bool DF = false,MF = false;

	int fragmentOffset;
	string currPacket;
	do{
		cin>>currPacket;

		ipHeaderLen = currPacket.substr(4,4);
		fragmentBlockNum = currPacket.substr(12,4);
		currData = currPacket.substr(40);

		data_len = stringHexToInt(ipHeaderLen) - 20;
		DF = stringHexToInt(fragmentBlockNum)>>14 & 0x1;
		MF = stringHexToInt(fragmentBlockNum)>>13 & 0x1;
		fragmentOffset = stringHexToInt(fragmentBlockNum) & (0x1FFF);

		if(DF){
			cout<<currData<<endl;
			return 0;
		}
		
		dataPackets[fragmentOffset] = currData;

	}while(MF);

	for(auto it:dataPackets){
		// cout<<it.second<<endl;
		for(int i=0;i<it.second.length();i+=2){
			cout<<(char)(charPairToInt(it.second[i],it.second[i+1]));
		}
	}
	cout<<endl;

	return 0;
}