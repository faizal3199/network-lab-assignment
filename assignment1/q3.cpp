#include <iostream>
#include <unistd.h>
using namespace std;

int main(){
	size_t pidStore;

	printf("%s\n","Starting program :)");

	pidStore = fork();

	if(pidStore == 0){
		printf("PID of child: %d\n",getpid());
	}
	else if(pidStore > 0){
		printf("PID of parent: %d\n",getpid());
	}
	else{
		printf("Why so many erros :( \n");
	}

	return 0;
}