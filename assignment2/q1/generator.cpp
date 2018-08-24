#include "bits/stdc++.h"
using namespace std;

int main(){
	string text,saved_text;

	string poly;
	
	cin>>text
	cin>>poly;

	saved_text = text;
	
	int poly_size = poly.size();
	
	int text_size = text.size();
	
	//Add text to genrate hash
	for(int i =0;i<poly_size-1;i++){
		text += '0';
	}

	for(int i =0;i<text_size;i++){
		if(text[i]=='1'){
			for(int j = 0;j<poly_size;j++){
				//XOR it
				if((text[i+j]=='1' && poly[j]=='1') || (text[i+j]=='0' && poly[j]=='0')){
					text[i+j] = '0';
				}
				else{
					text[i+j] = '1';
				}
			}
		}
	}

	//Output the original text
	cout<<saved_text
	//Output the CRC checksum hash
	cout<<text.substr(text.size()-poly.size()+1)<<endl;

	//Output the (original) polynomial
	cout<<poly<<endl;
}