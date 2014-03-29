/*
* this is my server test.  header file.
*				--likai, 2014.2.27
*
*/

/*--------------header file(s) included----------------*/
#ifndef _SERVER_H__
#define _SERVER_H__

#include"queue.h"
/*#include<pthread.h>*/


/*-------------------macro and consts------------------*/
/*const char fileName[] ="./ServerRec.log";*/
#define  MAX_QUEUE 1024
#define MAX_EVENTS 10000
#define  MAX_EPOLL_SIZE 10000
#define MAX_LOGBUFF_SIZE 2048
#define THREAD_NUM 2
#define  BUFFSIZE 1024

/*--------------Enum type declaration----------------*/
typedef enum ETYPE{
	INFO,
	WARN,
	ERROR
}ETYPE;

/*------------------function declaration---------------*/
/*read thread function*/
void* rTaskThreadFunc(void *args);
/*write thread function*/
void* wTaskThreadFunc(void *args);
/*log record thread function*/
void* logRecThread();
/*thread create function*/
 BOOL createThread(pthread_t *p, int size,  void* (*fp)(void* ), void *args);
/*set comm-type to non-blocking*/
 BOOL setSocketNoBlocking(int fd);
/*initial server config params*/
 BOOL initServer(int *sock, /*TaskQueue *rtq, TaskQueue *wtq,*/ int port);
/*display system fatal error*/
void sysEventLog( ETYPE e, const char* fName, const char* msgDesc, int errCode);
/*server main process function.*/
 BOOL serverProcess(int port);
 #endif