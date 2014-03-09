/*
*  my server test.   
*		likai, --2014.2.27
*/

/*--------------header files included------------------*/
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
/*#include<pthread.h>*/
#include<stdio.h>
#include<sys/time.h>
#include<string.h>
#include<fcntl.h>
#include"../headers/server.h"

/*-----------for debug------------------------------------------*/
/* #include"headers/debug.h"*/

/*-------------------macro and consts------------------*/
/*const char fileName[] ="./ServerRec.log";*/
/*-----------------------------------------------------*/

/*-----------------global variables--------------------*/
/*read threads array*/
pthread_t glbReadThread[2];
/*write threads array*/
pthread_t glbWriteThread[2];
/*log record thread*/
pthread_t glbLogRecThread;
/*read mutex.*/
pthread_mutex_t glbRMutex;
/*write mutex.*/
pthread_mutex_t glbWMutex;
/*record current connect-num*/
long glbNo;
/*log buffer*/
char logBuff[512];
/*------------------------------------------------------*/

/*-------------------function declaration---------------*/
/*read thread function*/
int readTaskThread()
{
	return -1;
}
/*write thread function*/
int writeTaskThread()
{
	return -1;
}
/*log record thread function*/
int logRecThread()
{
	return -1;
}
/*thread create function*/
int createThread(pthread_t p)
{
	return -1;
}
/*set socket comm-type to non-blocking*/
int setSocketNoBlocking(int sock)
{
	int opt;
	if((opt=fcntl(sock,F_GETFL))<0)
	{
		/*sysFatalErr("setNonBlocking:fcntl() --> ERR!\n");*/
		return NG;
	}
	opt = opt|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opt)<0)
	{
		/*sysFatalErr("setNonBlocking:fcntl() --> ERR!\n");*/
		return NG;
	}
	return	OK;
}
/*
 -- NOTES:
		initial server config params
 -- RETURNS:
		0 : normal return.
		char*: err return ,(error message). 
*/
int  initServer(int* sock, int port)
{
	/*create a net tcp socket.*/
	struct sockaddr_in addr;
	int ret=-1;
	;
	if ((ret = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		 sysFatalErr("initServer:socket() --> ERR!\n", ret);
		 return NG;
	}
	*sock = ret;
	/*set the params of struct addr.*/
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*binding the address to the socket.*/
	if ((ret=bind(*sock,(struct sockaddr*)&addr, sizeof(addr))) < 0)
	{
		sysFatalErr("initServer:socket() --> ERR!\n", ret);
		return NG;
	}

	/*set socket noblocking.*/
	if ((ret=setSocketNoBlocking(*sock)) < 0)
	{
		sysFatalErr("initServer:setSocketNoBlocking() --> ERR!\n", ret);
		return NG;
	}

	/*Set the socket to listen for connections*/
	if ((ret=listen(*sock, MAX_QUEUE))<0)
	{
		sysFatalErr("initServer:listen() --> ERR!\n", ret);
		return NG;
	}
	//Accomplish all things, return OK.
	return OK;
}
/*display system fatal error*/
void sysFatalErr(const char* errMsg, int errCode)
{
	time_t lt; /*define a longint time varible*/
	lt=time(NULL);/*system time and date*/
	//printf(ctime(&lt)); /*english format output*/
	//printf(asctime(localtime(&lt)));/*tranfer to tm*/
	ctime(&lt); /*english format output*/
	localtime(&lt);/*tranfer to tm*/
	printf("---%s\n",asctime(gmtime(&lt))); /*tranfer to Greenwich time*/

	printf("---ErrMsg:%s\n", errMsg);
	printf("---ErrCode:%d\n", errCode);
	printf("-------------------\n\n");
}
/*------------------------------------------------------*/

/*-----------------------函数声明------------------------*/







/*------------------------------------------------------*/



