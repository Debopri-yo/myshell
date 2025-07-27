#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_HISTORY 100
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
	char history[MAX_HISTORY][MAX_LINE];
	int pid;
	int status;
	int history_count=0;
	char cwd[MAX_LINE];
	while(1){
		getcwd(cwd,sizeof(cwd));
		printf("myshell:~%s:",cwd);
	if(!fgets(input,MAX_LINE,stdin)){
		break;
	}
	size_t len = strlen(input);
	if(len > 0 && input[len-1=='\n']){
		input[len-1]='\0';
	}
	int index=history_count % MAX_HISTORY;
	strcpy(history[index],input);
	history_count++;
	parse_input(input,args);
		if (args[0] == NULL) {
            continue;  
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }
	if (strcmp(args[0], "quit") == 0) {
            break;
        }

	if (strcmp(args[0],"history") == 0 ){
		int start = (history_count > MAX_HISTORY) ? (history_count - MAX_HISTORY) : 0;
		for ( int i = start;i < history_count;i++){
		       int index = i % MAX_HISTORY;
	       		printf("%d.%s\n",i+1,history[index]);
		}
	continue;
	}		



	if (strcmp(args[0], "cd") == 0){
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
	

