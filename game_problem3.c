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
#define GEN_SIZE 1000
#define BUFF_SIZE 100


void unlink_semaphores(){
	sem_unlink("/sema1");
	sem_unlink("/sema2");
	sem_unlink("/sema3");
	sem_unlink("/sema4");	
}

void close_pipe(int fd[2]){
	close(fd[0]);
	close(fd[1]);
}

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
	long time_long;
	time_t time_var;
	char temp1[BUFF_SIZE];
	int pipe1[2],pipe2[2];
	char player1_data[BUFF_SIZE];
	char player2_data[BUFF_SIZE];

	pid_t player1_pid,player2_pid;
	sem_t *sem1,*sem2,*sem3,*sem4;
	
	unlink_semaphores();
	
	sem1 = sem_open("/sema1", O_CREAT,  0644, 1);
	sem2 = sem_open("/sema2", O_CREAT,  0644, 1);
	sem3 = sem_open("/sema3", O_CREAT,  0644, 0);
	sem4 = sem_open("/sema4", O_CREAT,  0644, 0);

	pipe(pipe1);
	pipe(pipe2);
	
	player1_pid = fork();
	if(player1_pid>0)
		player2_pid = fork();
		
			
	if(player1_pid == 0 ){
		srand((unsigned) time(&time_var));
		for(i=0;i<n;i++){
			sem_wait(sem1);	
			temp = rand()%GEN_SIZE;
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
		
			temp = rand()%GEN_SIZE;
			printf("Player 2 selected : %d\n",temp);		
			sprintf(temp1,"%d",temp);
			close(pipe2[0]);
			len2=strlen(temp1);
			write(pipe2[1],temp1,BUFF_SIZE);
			sem_post(sem4);
			
		}
	}else{
		
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
	unlink_semaphores();
	close_pipe(pipe1);
	close_pipe(pipe2);
}
int main(int argc,char *args[]){
	int n;
	if(argc>=2){
		printf("\n2 Player Game ");
		n = atoi(args[1]);
		printf("\nNo. of Iterations : %d\n\n",n);
		game(n);
	}else
		printf("Please enter no of iterations in argument\n");
	return 0;
}
