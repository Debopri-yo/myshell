#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_ARGS 64
void parse_input( char *input,char **args){
int i=0;
args[i]=strtok(input," \t\n");
while(args[i]!=NULL){
	i++;
	args[i]=strtok(NULL," \t\n");
}
}
int main(){
	char input[MAX_LINE];
	char *args[MAX_ARGS];
	int pid;
	int status;
	while(1){
		printf("myshell> ");
	if(!fgets(input,MAX_LINE,stdin)){
		break;
	}
	parse_input(input,args);
		if (args[0] == NULL) {
            continue;  
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }
	
	if (strcmp(args[0], "cd") == 0){
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("current working directory %s\n",cwd);
		printf("trying to cd into %s\n",args[1]);
		if (args[1] == NULL){
			fprintf(stderr,"cd: missing arguement\n");
		}
		else if (chdir(args[1]) == -1){
			perror("chdir failed");
		}
		continue;
	}

		pid=fork();
			if(pid < 0){
				perror("fork failed");
			}
			else if(pid == 0){
				if(execvp(args[0],args)<0){
					perror("exec failed");
					exit(1);
			}
			}
			else{
			waitpid(pid,&status,0);
			}
	}
	return 0;
}
	

