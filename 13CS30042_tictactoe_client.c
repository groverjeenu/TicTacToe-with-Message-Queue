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
    while(s[iii]!='\t' && s[iii]!='\0')
    {
        pid_send += s[iii];
        iii++;
    }
    pid_send[iii] = '\0';
    iii++;

    while(s[iii]!='\0')
    {
        msg_new += s[iii];
        iii++;
    }

    pid_int = atoi(pid_send);

    return pair<int,char *>(pid_int,msg_new);
}

int main()
{

    int msgid = msgget(KEY,IPC_CREAT|0666);
    if(msgid  < 0)printf("msgget Error\n");
    struct msgStruct mymsg;

    int pid1 = getpid() ;
    char con[1000];
    scanf("%s",con);
    cout<<con<<endl;

    char ss1[1000], temp1[1000];
        sprintf(temp1,"%d\t",pid1);
        //printf("Player1>");
        //scanf("%s",ss);
        strcat(temp1,con);

    mymsg.mtype = pid1;
        strcpy(mymsg.mtext,temp1);
        cout<<mymsg.mtext<<endl;

        if(msgsnd(msgid,&mymsg,MSGLEN,0) < 0 )cout<<"msgsnd Error"<<endl;



    while(1)
    {
        char ss[1000], temp[1000];
        sprintf(temp,"%d\t",pid1);
        printf("Player PID : %d >",pid1);
        scanf("%s",ss);
        strcat(temp,ss);

        mymsg.mtype = pid1;
        strcpy(mymsg.mtext,temp);

        if(msgsnd(msgid,&mymsg,MSGLEN,0) < 0 )cout<<"msgsnd Error"<<endl;




    }
}
