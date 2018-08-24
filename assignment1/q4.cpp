#include <iostream>
#include <unistd.h>
using namespace std;

int main(){
	size_t pid;

	pid = fork();

	if(pid == 0){
		printf("First child: %d\n",getpid());
		fflush(stdout);

		pid = fork();

		if(pid == 0){
			printf("First grandchild of first child: %d\n",getpid());
			fflush(stdout);
		}
		else{
			pid = fork();
			if(pid == 0){
				printf("Second grandchild of first child: %d\n",getpid());
				fflush(stdout);
			}
		}
	}
	else{
		printf("Original process: %d\n",getpid());
		fflush(stdout);

		pid = fork();

		if(pid == 0){
			printf("Second child: %d\n",getpid());
			fflush(stdout);

			pid = fork();

			if(pid == 0){
				printf("First grandchild of second child: %d\n",getpid());
				fflush(stdout);
			}
			else{
				pid = fork();
				if(pid == 0){
					printf("Second grandchild of second child: %d\n",getpid());
					fflush(stdout);
				}
			}
		}
	}
	// sleep(100);
	return 0;
}