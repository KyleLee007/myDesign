/*
*  my server test.   
*		likai, --2014.2.27
*/

/*--------------header files included------------------*/
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include <errno.h>
/*#include<pthread.h>*/
#include<sys/epoll.h>
#include<stdio.h>
#include<sys/time.h>
#include<string.h>
#include<fcntl.h>
#include <arpa/inet.h>

#include"../headers/server.h"
#include"../headers/queue.h"
/*-----------for debug------------------------------------------*/
/*#include"headers/debug.h"*/

/*-------------------macro and consts------------------*/
/*const char fileName[] ="./ServerRec.log";*/
/*-----------------------------------------------------*/

/*-----------------global variables--------------------*/
/*client count.*/
unsigned long glbNo;
/*log buffer*/
char logBuff[MAX_LOGBUFF_SIZE];
/*sample send content.*/
const char htmlContent[] = {
	"<head>"
		"<title>linux epoll server</title>"
		"</head>"
		"<body>"
		"Hello, you are %luth user!"
		"</body>"
};

/*-------------------function declaration---------------*/
/*read task thread function*/
void* rTaskThreadFunc(void *args)
{
	int curFD = -1;
	char recBuff[BUFFSIZE]={0};
	int ret=-1;
	 BOOL readAgain;
	TaskParams *rTP = (TaskParams*)args;
	/*Infinite loop.*/
	while(TRUE)
	{
		readAgain = TRUE;
		/*dequeue the first elem.*/
		/*in deQueue() we'll judge if the quque is empty then wait.*/
		if ( !deQueue( rTP, &curFD) )
		{
			sysEventLog(ERROR, "readTaskThread()", "deQueue() returned error.", ret);
			break;
		}

		/*receive the client's data.*/
		while (readAgain)
		{
			ret = recv(curFD, recBuff, BUFFSIZE, 0);
			if (ret<0)
			{
				/* Because of non-blocking mode, if errno is EAGAIN, it
				*means that there is no data in data-buffer.So we break.*/
				if (EAGAIN == errno)
				{
					sysEventLog(WARN, "readTaskThread()", "errno is EAGAIN.", errno);
					break;
				} 
				else/*else, recv() return error.*/
				{
					sysEventLog(WARN, "readTaskThread()", "recv() returnd error.", ret);
					/*DO NOT forget close the socket.*/
					close(curFD);
					break;
				}
			}
			/*we think the client socket is closed.*/
			else if(0 == ret)
			{
				readAgain = FALSE;
			}
			/*Need to read again?*/
			readAgain = (BUFFSIZE==ret ?  TRUE : FALSE);
		}
		if (ret > 0)
		{
			/*here do something to deal the data we received.
			*	such as a example:
			*				printf("%d send:%s.\n",curFD, recBuff);
			*/
			printf("\n+=+=+=+=+=+DBG(%lu):I receive %d send:%s.\n\n", pthread_self() , curFD, recBuff);
			close(curFD);
		}
	}
	return NULL;
}

/*read task thread function*/
void* wTaskThreadFunc(void *args)
{
	char sendBuff[BUFFSIZE]={0};
	char tmpBuff[512]={0};
	int ret = -1;
	int curFD = -1;
	TaskParams *wTP = (TaskParams*)args;
	
	/*Infinite loop.*/
	while (TRUE)
	{
		/*dequeue the first elem.*/
		/*in deQueue() we'll judge if the quque is empty then wait.*/
		if ( !deQueue(wTP, &curFD) )
		{
			sysEventLog(ERROR, "writeTaskThread()", "deQueue() returned error.", ret);
			break;
		}

		/*Format the sent data.
		  *	  http version: 1.0.
		  *	  status-code: 200.
		  *	  Reason-Phrase: OK.
		  *	  Content-type:text/html(standard html format).
		  *  reference : http://www.alixixi.com/Dev/Serv/WebSvr/2007/2007111779091.html.
		  */
		sprintf(tmpBuff,htmlContent,glbNo);
		sprintf(sendBuff, "HTTP/1.0 200 OK\r\nContent-type:text/html\r\n%s",tmpBuff);
		if ((ret=send(curFD, sendBuff, strlen(sendBuff)+1, 0)) < 0)
		{
			sysEventLog(WARN, "writeTaskThread()", "send() returned error.", ret);
		}
		close(curFD);
	}
	return NULL;
}

/*log record thread function*/
void* logRecThread()
{
//	return -1;

	return NULL;
}

/*thread create function*/
 BOOL createThread(pthread_t *p, int size, void* (*fp)(void* ), void *args)
{
	int i=0;
	int ret=-1;
	/*tmp log buffer.*/
	char buf[128]={0};
	/*func is symbol of function name:
			U : unknown function.
			R: read task function.
			W:write task function.
	*/
	char func='U';

	for(; i<size; i++)
	{
		if( (ret=pthread_create(p+i, NULL, fp, args)) )
		{
			/*Judge whether the function is read or write.*/
			func = (rTaskThreadFunc == *fp ? 'R' : 'W');
			/*prepare the error message.*/
			sprintf(buf, "error occured when create %c[%d] thread.",func, i);
			sysEventLog(ERROR, "createThread", buf, ret);
			 return FALSE;
		}
	}
	return TRUE;
}

/*set socket comm-type to non-blocking*/
 BOOL setSocketNoBlocking(int sock)
{
	int opt;
	if((opt=fcntl(sock,F_GETFL))<0)
	{
		/*sysFatalErr("setNonBlocking:fcntl() --> ERR!\n");*/
		return FALSE;
	}
	opt = opt|O_NONBLOCK;
	if(fcntl(sock, F_SETFL, opt)<0)
	{
		/*sysFatalErr("setNonBlocking:fcntl() --> ERR!\n");*/
		return FALSE;
	}
	return	TRUE;
}

/*==========================
*-- NOTES:
*		initial server config.
*-- RETURNS:
*		TRUE : normal return.
*		FALSE: error return.
*==========================*/
 BOOL  initServer(int *sock, /*TaskQueue *rtq, TaskQueue *wtq,*/ int port)
{
	/*create a net tcp socket.*/
	struct sockaddr_in addr;
	int ret=-1;

	if ((ret = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		 sysEventLog(ERROR, "initServer()", "socket() returnd error.", ret);
		 return FALSE;
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
		sysEventLog(ERROR, "initServer()", "bind() returnd error.", ret);
		return FALSE;
	}

	/*set socket noblocking.*/
	if (!(ret=setSocketNoBlocking(*sock)))
	{
		sysEventLog(ERROR, "initServer()", "setSocketNoBlocking() return error.", ret);
		return FALSE;
	}

	/*Set the socket to listen for connections*/
	if ((ret=listen(*sock, MAX_QUEUE))<0)
	{
		sysEventLog(ERROR, "initServer()", "listen() returned error.", ret);
		return FALSE;
	}
	/*Accomplish socket initialize, now begin to initial task queue.*/
  /*  WE MUST INITIAL rtq and wtq before create read and write task threads.
	*  or isEmpty(rtq) and isEmpty(wtq) will return FALSE at function rTaskThreadFunc(line57)
	* and function wTaskThreadFunc(line123), so ERROR occurred.
	*/
/*	if (initQueue(rtq) != TRUE)
	{
		sysEventLog(ERROR, "initServer()", "Init read-task-queue.", FALSE);
		return FALSE;
	}
	if (initQueue(wtq) != TRUE)
	{
		sysEventLog(ERROR, "initServer:initQueue()", "Init write-task-queue.", FALSE);
		return FALSE;
	}*/
	return TRUE;
}

/*display system fatal error*/
void sysEventLog( ETYPE e, const char* fName, const char* msgDesc, int errCode)
{
	static time_t lt; /*define a longint time varible*/
	lt=time(NULL);/*system time and date*/
	//printf(ctime(&lt)); /*english format output*/
	//printf(asctime(localtime(&lt)));/*tranfer to tm*/
	ctime(&lt); /*english format output*/
	
/*	switch(e)
	{
	case INFO:break;
	case WARN: break;
	case  ERROR: break;
	}
*/
	printf("========================\n"
				"---TIME:%s"
				"---ERRMSG:%s.%s\n"
				"---ERRCODE:%d\n"
				"-------------------------------------------\n\n",
				asctime(localtime(&lt)),
				fName, msgDesc,
				errCode
		);
}

/*server main process.*/
BOOL serverProcess(int port) 
{
	/*----------------------------variables define.-------------------------------------------------*/
	/*epoll's fd.*/
	register int epollFD =-1;
	/*tmp client epoll-event*/
	struct epoll_event tmpEv;
	/*epoll-event collection.*/
	struct epoll_event events[MAX_EVENTS];
	/*new client socket*/
	struct sockaddr_in clientAddr;
	/*other vars*/
	char *str=NULL;
	int clientSocket;
	int evCnt =-1;
	int i=0;
	int ret = -1;
	int listenSocket = 0;
   /* int client = 0;*/
	socklen_t addrLen = sizeof(struct sockaddr_in);
	/*read threads array*/
	pthread_t rTaskThreadIDs[THREAD_NUM];
	/*read task params struct.*/
	TaskParams rTP;

	/*write threads array*/
	pthread_t wTaskThreadIDs[THREAD_NUM];
	/*write task params struct.*/
	TaskParams wTP;

	/*log record thread*/
/*	pthread_t logRecThreadID;*/

/*----------------------------params initialize.--------------------------------------------------------*/
	/*init r&w's queue.*/
	if ( (str=initQueue(&(rTP))) ||
		  (str=initQueue(&(wTP)))
	   )
	{
		sysEventLog(ERROR, "serverProcess()", str, FALSE);
	}

	/*Create r&w task thread.*/
	if ( ! createThread(rTaskThreadIDs, THREAD_NUM, rTaskThreadFunc, &(rTP)) ||
		 ! createThread(wTaskThreadIDs, THREAD_NUM, wTaskThreadFunc, &(wTP))
	   )
	{
		/*sysEventLog(ERROR, "serverProcess()", " create task thread error.", FALSE);*/
		/*ERROR/WARN log has been recorded in initServer().*/
		return FALSE;
	}	

    /* Initialize the server */
    if ( ! initServer(&listenSocket, /*&(rTP.taskQue), &(wTP.taskQue),*/ port))
    {
		/*ERROR/WARN log has been recorded in initServer().*/
		return FALSE;
    }

	/*Setup epoll variables*/
	if ((ret=epoll_create(MAX_EPOLL_SIZE)) < 0)
	{
		sysEventLog(ERROR, "serverProcess()", "epoll_create() returned error.", ret);
		return FALSE;
	}
	epollFD = ret;
	tmpEv.events = EPOLLIN|EPOLLET;
    tmpEv.data.fd = listenSocket;

	if ((ret=epoll_ctl(epollFD, EPOLL_CTL_ADD, listenSocket, &tmpEv)) < 0)
    {
        sysEventLog(ERROR, "serverProcess()", "epoll_ctl() returned error.", ret);
		return FALSE;
    }
/*---------------------------server working loop.----------------------------------------------------------*/
	/*Infinite loop.*/
	while (TRUE)
	{
		/*Wait for epoll to return the events.*/
		if ((evCnt = epoll_wait(epollFD, events, MAX_EVENTS, -1))<0)
		{
			sysEventLog(ERROR, "serverProcess()", "epoll_wait() returned error.", evCnt);
			return FALSE;
		}

		/*loop to deal these events.*/
		for (i=0; i<evCnt; i++)
		{
			/*If it is listen fd, it means a new connection join in.*/
			if (events[i].data.fd == listenSocket)  
			{
				/*Accept the new connection.*/
				if ((clientSocket=accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen))<0)
				{
					sysEventLog(ERROR, "serverProcess()", "accept() returned error.", clientSocket);
					return FALSE;
				}
				/*Same as listenSocket, set no-blocking mode.*/
				if ( ! (ret=setSocketNoBlocking(clientSocket)))
				{
					sysEventLog(ERROR, "serverProcess()", "setSocketNoBlocking() returned error.", ret);
					return FALSE;
				}
				/**---------------------*for debug.**-----------------------*/
				str = inet_ntoa(clientAddr.sin_addr);
				printf("\n\nclient addr is %s \n\n\n", str);
				/**----------------------**end**---------------------------**/
				/*add the new-socket in tmp struct variable.*/
				tmpEv.data.fd = clientSocket;
				/*register the events for new-socket.*/
				tmpEv.events = EPOLLIN|EPOLLOUT;
				/*add tmpEV in events[] array collection.*/
				if ((ret= epoll_ctl(epollFD, EPOLL_CTL_ADD, clientSocket, &tmpEv))<0)
				{
					sysEventLog(ERROR, "serverProcess()", "epoll_ctl() returned error.", ret);
				}
				/*done.*/
			} 
			/*if the existing fd has event, we must judge it is legal or not.*/
			else if (events[i].data.fd >0)
			{
				/*if the event is EPOLLIN, it means the fd is readable.*/
				if (events[i].events & EPOLLIN)
				{
					/*enter the read-task-queue.*/
					if ( ! (ret=enQueue(&rTP, events[i].data.fd)) )
					{
						sysEventLog(ERROR, "serverProcess()", "enQueue(rTaskQueue) returned error.",ret);
						return FALSE;
					}
				}
				/*otherwise if EPOLLOUT, this fd can be written.*/
				else if(events[i].events & EPOLLOUT)
				{
                printf("I got EOPLLOUT! rTP len=%d, wTP len=%d\n", rTP.taskQue.length, wTP.taskQue.length);
					/*enter the write-task-queue.*/
		/*			if ( ! (ret=enQueue(&wTP, events[i].data.fd)) )
					{
						sysEventLog(ERROR, "serverProcess()", "enQueue(wTaskQueue) returned error.",ret);
						return FALSE;
					}*/
				}
				/*else, the event is other(EPOLLERR,EPOLLHUP...).*/
				else
				{
					/*do nothing.*/
				}
			}
			else /*else , the fd we receive is illegal.*/
			{
				sysEventLog(ERROR, "serverProcess()", "The reveived FD is illegal.", events[i].data.fd);
				return FALSE;
			}
		}
	}
}
