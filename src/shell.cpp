#include<iostream>
#include<string>
#include<sstream>
#include<unistd.h>
#include<vector>
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
		istringstream iss(input);
		vector<string> tokens;
		string token;
		while(iss >>  token) tokens.push_back(token);
		vector<char*>args;
		for(auto& t : tokens) args.push_back(&t[0]);
		args.push_back(nullptr);
		execvp(args[0],args.data());
		cerr<<"Unknown command"<<endl;
		_exit(1);
	}
	else{
		wait(NULL);
	}
}
return 0;
}


