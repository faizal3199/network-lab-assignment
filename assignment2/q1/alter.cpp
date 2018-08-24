#include "bits/stdc++.h"
using namespace std;

int main(){
	string text,poly;

	cin>>text;
	cin>>poly;

	// Flip bits
	if(text[0]=='1'){
		text[0]='0';
	}
	else{
		text[0]='1';
	}
	
	cout<<text<<endl;
	
	cout<<poly<<endl;
	return 0;
}