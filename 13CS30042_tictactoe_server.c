//Jeenu Grover
//13CS30042
//M/C No. : 46
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define KEY 2025
#define MSGLEN 5000
#define SERVER_TYPE 123456

typedef pair<int, char *> pss;

char grid[3][3];

struct msgStruct
{
    long mtype;
    char mtext[MSGLEN];


};

pair<int,char *> extract(char s[])
{
    char* pid_send, *msg_new;
    pid_send = (char *)malloc(1000*sizeof(char));
    msg_new = (char *)malloc(1000*sizeof(char));
    int pid_int,iii = 0;
    int ij,jk;
    ij = 0;
    jk = 0;
    while(s[iii]!='\t' && s[iii]!='\0')
    {
        pid_send[ij++]= s[iii];
        iii++;
    }
    pid_send[iii] = '\0';
    iii++;

    while(s[iii]!='\0')
    {
        msg_new[jk++] = s[iii];
        iii++;
    }

    pid_int = atoi(pid_send);

    return pair<int,char *>(pid_int,msg_new);
}

int validate()
{
	int i,j,k;
	char cc = '_';

	for( i = 0;i<3;i++)
	{
		if(grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2] && grid[i][0] != '_')
		{
			cc = grid[i][0];
			break;
		}
	}

	if(cc == '_')
	{
		for( i = 0;i<3;i++)
		{
			if(grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i] && grid[0][i] != '_')
			{
				cc = grid[0][i];
				break;
			}
		}
	}
	if(cc == '_')
	{
			if(grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2] && grid[0][0] != '_')
			{
				cc = grid[0][0];
				//break;
			}
		
	}
	if(cc == '_')
	{
			if(grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0] && grid[0][2] != '_')
			{
				cc = grid[1][1];
				//break;
			}
		
	}

	if(cc != '_')
	{
		if(cc == 'X')
		{
			printf("Player 1 wins\n");
			return 1;
		}
		else
		{
			printf("Player 2 wins\n");
			return 1;
		}
	}
	int flag = 0;

	for(i = 0;i<3;i++)
	{
		for(j = 0;j<3;j++)
		{
			if(grid[i][j] == '_')
			{
				flag = 1;
				break;
			}
		}
	}

	if(flag == 0){
		printf("Draw\n");
		return 1;
	}

	return 0;
}


int main()
{

	int msgid = msgget(KEY,IPC_CREAT|0666);
    if(msgid  < 0)printf("msgget Error\n");
    struct msgStruct mymsg;

    int flag1 = 0,flag2 = 0,pid1,pid2;

    while(flag1 == 0)
    {
    	if(msgrcv(msgid,&mymsg,MSGLEN,0,0) < 0 ) cout<<"msgrcv Error"<<endl;
    	cout<<mymsg.mtext<<endl;
    	pss re = extract(mymsg.mtext);
    	cout<<re.second<<endl;

    	if(strncmp("connect",re.second,6) == 0)
    	{
    		printf("Player 1 Joined,Assigned X\n");
    		pid1 = re.first;
    		flag1 = 1;
    	}

    }

    while(flag2 == 0)
    {
    	if(msgrcv(msgid,&mymsg,MSGLEN,0,0) < 0 ) cout<<"msgrcv Error"<<endl;
    	pss re = extract(mymsg.mtext);

    	if(strncmp("connect",re.second,7) == 0)
    	{
    		printf("Player 2 Joined,Assigned O\n");
    		pid2 = re.first;
    		flag2 = 1;
    	}

    }

    int i,j,k;

    printf("GAME BEGINS\n");
    for(i = 0;i<3;i++)
    	for ( j= 0;j<3;j++)
    		grid[i][j] = '_';

    for( i = 0 ;i <3 ;i++)
    {
    	printf("%c\t%c\t%c\n",grid[i][0],grid[i][1],grid[i][2]);
    }

    int c = 0;
    int p ,q;
    while(1)
    {
    	if(c == 0)
    	{
    		printf("Player PID : %d ,its your turn\n",pid1);
    		if(msgrcv(msgid,&mymsg,MSGLEN,0,0) < 0 ) cout<<"msgrcv Error"<<endl;

    		pss re = extract(mymsg.mtext);
    		if( sscanf(re.second,"%d,%d",&p,&q) < 0 || grid[p][q] != '_')
    		{
    			printf("INAVLID INPUT\n");
    			continue;
    		}
            printf("%d %d\n",p,q);

    		if(re.first == pid1)
    		{
    			grid[p][q] = 'X';
    			c = 1-c;
    			for( i = 0 ;i <3 ;i++)
			    {
			    	printf("%c\t%c\t%c\n",grid[i][0],grid[i][1],grid[i][2]);
			    }
    			if(validate())
    				{
    					kill(pid1,SIGTERM);
    					kill(pid2,SIGTERM);
    					printf("Game Ended\n");
    					return 0;
    				}
    		}

    	}
    	if(c == 1)
    	{
    		printf("Player PID : %d ,its your turn\n",pid2);
    		if(msgrcv(msgid,&mymsg,MSGLEN,0,0) < 0 ) cout<<"msgrcv Error"<<endl;

    		pss re = extract(mymsg.mtext);
    		if( sscanf(re.second,"%d,%d",&p,&q) < 0 || grid[p][q] != '_')
    		{
    			printf("INAVLID INPUT\n");
    			continue;
    		}
            printf("%d %d\n",p,q);
    		if(re.first == pid2)
    		{
    			grid[p][q] = 'O';
    			for( i = 0 ;i <3 ;i++)
			    {
			    	printf("%c\t%c\t%c\n",grid[i][0],grid[i][1],grid[i][2]);
			    }
    			c = 1-c;
    			if(validate())
    				{
    					kill(pid1,SIGTERM);
    					kill(pid2,SIGTERM);
    					printf("Game Ended\n");
    					return 0;
    				}
    		}

    	}
    }



}