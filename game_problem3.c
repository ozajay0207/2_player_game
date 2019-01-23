#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define INT_SIZE 1000
#define BUFF_SIZE 100
int make_decision(int num1,int num2){
	int max_min_flag = rand()%2;
	static int score_1=0;
	static int score_2=0; 
	if(max_min_flag == 1){
		printf("Winner is Maximum Number\n");
		if(num1>num2){
			score_1++;
		}else{
			score_2++;
		}
	}else{
		printf("Winner is Minimum Number\n");
		if(num1<num2){
			score_1++;
		}else{
			score_2++;
		}
	}
	printf("Score : %d %d\n\n",score_1,score_2);
	if(score_1==score_2)
		return 0;
	else if(score_1>score_2)
		return 1;
	else 
		return 2;
}
void game(int n){
	int len1,len2,i,val=-1,winner=0;
	int temp;
	char temp1[BUFF_SIZE];
	int pipe1[2],pipe2[2];
	char player1_data[BUFF_SIZE];
	char player2_data[BUFF_SIZE];
	pid_t player1_pid,player2_pid;
	
	sem_t *sem1,*sem2,*sem3,*sem4;
	
	
	sem_unlink("/semaphore1");
	sem_unlink("/semaphore2");
	sem_unlink("/semaphore3");
	sem_unlink("/semaphore4");	
	sem1 = sem_open("/semaphore1", O_CREAT,  0644, 1);
	sem2 = sem_open("/semaphore2", O_CREAT,  0644, 1);
	sem3 = sem_open("/semaphore3", O_CREAT,  0644, 0);
	sem4 = sem_open("/semaphore4", O_CREAT,  0644, 0);

	pipe(pipe1);
	pipe(pipe2);
	
	player1_pid = fork();
	if(player1_pid>0)
		player2_pid = fork();
		
		
	if(player1_pid == 0 ){
		for(i=0;i<n;i++){		
			sem_wait(sem1);		
			temp = rand()%100;
		
			printf("Player 1 selected : %d\n",temp);
			sprintf(temp1,"%d",temp);
			close(pipe1[0]);
			len1=strlen(temp1);
			write(pipe1[1],temp1,BUFF_SIZE);					
			sem_post(sem3);				
		}
	}else if(player2_pid == 0){
		for(i=0;i<n;i++){
			
			sem_wait(sem2);
			temp = rand()%101;
		
			printf("Player 2 selected : %d\n",temp);		
			sprintf(temp1,"%d",temp);
			close(pipe2[0]);
			len2=strlen(temp1);
			write(pipe2[1],temp1,BUFF_SIZE);
			sem_post(sem4);
			
		}
	}else{

//		pid_t child_pid = waitpid(player1_pid,&stat,0);		
//		pid_t child_pid1 = waitpid(player2_pid,&stat,0);
		
		for(i=0;i<n;i++){		
		
		sem_wait(sem3);
		sem_wait(sem4);

		close(pipe1[1]);
		read(pipe1[0],player1_data,BUFF_SIZE);
		close(pipe2[1]);
		read(pipe2[0],player2_data,BUFF_SIZE);			
		
		//printf("In parent - Player 1: %d\n",atoi(player1_data));	
		//printf("In parent - Player 2: %d\n\n",atoi(player2_data));	

		winner = make_decision(atoi(player1_data),atoi(player2_data));
		
		if(i==n-1){
		if(winner == 0){
			printf("It is a Tie\n");
		}else if(winner == 1){
			printf("Winner is Player 1\n");
		}else{
			printf("Winner is Player 2\n");
		}}
		
		sem_post(sem1);	
		sem_post(sem2);	


		}				
		
	}
	
	

	sem_close(sem1);
        sem_close(sem2);
        sem_close(sem3);
        sem_close(sem4);
	sem_unlink("/semaphore1");
	sem_unlink("/semaphore2");
	sem_unlink("/semaphore3");
	sem_unlink("/semaphore4");
}
int main(int argc,char *args[]){
	static int n=1;
	//n = (int)args[1];
		game(10);
	//	}

	return 0;
}
