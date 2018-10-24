#include<bits/stdc++.h>
#include<iostream>
#include<string>


using namespace std;

string intToStringHex(int x,int fill = 4){
	stringstream s;
	s << setfill('0') << setw(fill) << hex << x;
	string temp = s.str();
	temp = temp.substr(temp.length()-fill,fill);
	// transform(temp.begin(), temp.end(),temp.begin(),::toupper);
	return temp;
}

vector<string> ipToHex(string ip){
	string data = "";

	vector<string> ret_value;

	int i = 0;
	while(i<ip.length()){
		if(ip.at(i) == '.'){
			ret_value.push_back(intToStringHex(stoi(data),2));
			data = "";
			i++;
			continue;
		}
		data.push_back(ip.at(i));
		i++;
	}

	ret_value.push_back(intToStringHex(stoi(data),2));

	return ret_value;
}

int main(){
	string ip;
	cout<<"Enter IPv4:"<<endl;
	cin>>ip;
	vector<string> ipv6_temp = ipToHex(ip);

	string ipv6 = "";
	for(int i=0;i<ipv6_temp.size();i++){
		if(i%2 == 0){
			ipv6.push_back(':');
		}
		ipv6 += ipv6_temp[i];
	}

	ipv6 = "ffff" + ipv6;
	// ipv6 = "0:0:0:0:0:" + ipv6;
	ipv6 = "::"+ipv6;
	cout<<"IPv6 for "<<ip<<" is"<<endl;
	cout<<ipv6<<endl;
	return 0;
}
