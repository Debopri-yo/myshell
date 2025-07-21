#include<iostream>
#include<string>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
int main(){
	string input;
	cout<<"wellcome to myshell"<<endl;
	while(true){
		cout<<"myshell> ";
		getline(cin,input);
		if(cin.eof()){
		cout<<"exiting myshell"<<endl;
		break;
	}
		if(input.empty()){
			continue;
		}
	if(input=="exit"||input=="quit"){
		cout<<"goodbye"<<endl;
		break;
	}
	pid_t pid=fork();
	if (pid<0){
		cerr<<"fork failed"<<endl;
	}
	else if (pid==0){
		cout<<"[Child] PID: "<<getpid()<<"Running | "<<input<<endl;
		_exit(0);
	}
	else{
		int status;
		wait(&status);
		cout<<"[Parent] child finished with status"<<status<<endl;
	}
}
return 0;
}


