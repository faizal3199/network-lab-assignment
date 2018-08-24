#include<iostream>
using namespace std;

int main(int argv, char** argc){
	if(argv != 4){
		printf("Usage:\n%s x (+|-|/|*) y\n",argc[0]);
		return 1;
	}
	int a = atoi(argc[1]);
	int b = atoi(argc[3]);

	char* op = argc[2];

	int results;
	switch(*op){
		case '+':
			results = a+b;
			break;
		case '-':
			results = a-b;
			break;
		case '*':
			results = a*b;
			break;
		case '/':
			results = a/b;
			break;
		default:
			printf("%s\n","Invalid operator. See usage.");
			return 1;
	}
	printf("Result is %d\n",results);
	return 0;
}