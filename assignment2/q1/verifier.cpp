#include "bits/stdc++.h"
using namespace std;

int main(){
	string text;

	string poly;
	
	cin>>text;
	cin>>poly;
	
	int poly_size = poly.size();

	int text_size = text.size();
	
	text_size -= poly_size-1;//Get the original length
	
	for(int i = 0;i<text_size;i++){
		if(text[i]=='1'){
			for(int j = 0;j<poly_size;j++){
				//XOR again to get the original data as the xor is reversible
				if((text[i+j]=='1'&&poly[j]=='1') || (text[i+j]=='0'&&poly[j]=='0')){
					text[i+j] = '0';
				}
				else{
					text[i+j] = '1';
				}
			}
		}
	}

	for(int i = text_size;i<text.size();i++){
		if(text[i]=='1'){
			cout<<"Data is not correct"<<endl;return 0;
		}
	}
	
	cout<<"Data is correct"<<endl;
}