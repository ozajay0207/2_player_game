#include<stdio.h>
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/wait.h> 
#include<string.h>
#define INT_SIZE 1000
#define BUFF_SIZE 100

void game(int n){
	int len1,len2;
	int stat,i;
	int temp;
	char temp1[BUFF_SIZE];
	int pipe1[2];
	int pipe2[2];
	char player1_data[BUFF_SIZE];
	char player2_data[BUFF_SIZE];
	pid_t player1_pid,player2_pid;
	
	pipe(pipe1);
	pipe(pipe2);

	player1_pid = fork();
	if(player1_pid>0)
		player2_pid = fork();
	
	for(i=0;i<n;i++){
	if(player1_pid == 0 ){
		
		temp = rand()%1000;
		sprintf(temp1,"%d",temp);
		printf("\nA selected : %s",temp1);
		close(pipe1[0]);
		len1=strlen(temp1);
		write(pipe1[1],temp1,len1);
		//close(pipe1[1]);
		//sleep(1);
	}else if(player2_pid == 0){
		//for(i=0;i<n;i++){
		temp = rand()%1001;
		sprintf(temp1,"%d",temp);
		printf("\nB selected : %s",temp1);		
		close(pipe2[0]);
		len2=strlen(temp1);
		write(pipe2[1],temp1,len2);
		//sleep(1);
		//close(pipe2[1]);
		//}
	}else{

//		for(i=0;i<3;i++){

		pid_t child_pid = waitpid(player1_pid,&stat,0);
		close(pipe1[1]);
		read(pipe1[0],player1_data,BUFF_SIZE);
		
		pid_t child_pid1 = waitpid(player2_pid,&stat,0);
		close(pipe2[1]);
		read(pipe2[0],player2_data,BUFF_SIZE);
		
		//close(pipe1[0]);
		//close(pipe2[0]);
		

		printf("\nPlayer 1: %s \nPlayer 2:%s\n",player1_data,player2_data);		
//		}
	}
	}
}
int main(int argc,char *args[]){
	static int n=1;
	//n = (int)args[1];
	//while(n--){
		//printf("\nIteration : %d",n);
		game(3);
	//	sleep(1);

	//	}
	return 0;
}
