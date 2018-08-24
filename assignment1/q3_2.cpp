#include <iostream>
#include <unistd.h>
using namespace std;

int main(){
	size_t pidStore;

	printf("%s\n","Starting program");
	fflush(stdout);

	for(int i=0;i<5;i++){
		printf("Loop index > %d\n",i);
		fflush(stdout);
		pidStore = fork();

		if(pidStore == 0){
			printf("PID of child: %d\n",getpid());
			fflush(stdout);
		}
		else if(pidStore > 0){
			printf("PID of parent: %d\n",getpid());
			fflush(stdout);
		}
		else{
			printf("Why so many erros\n");
		}
	}

	return 0;
}