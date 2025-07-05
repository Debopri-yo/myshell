#include<iostream>
#include<string>
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
	cout<<"you typed"<<input<<endl;

	}
return 0;
}

