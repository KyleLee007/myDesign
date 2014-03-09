/*
* this is my server test.  header file.
*				--likai, 2014.2.27
*
*/
/*--------------header file(s) included----------------*/
#include"queue.h"
#include<pthread.h>
/*-------------------macro and consts------------------*/
/*const char fileName[] ="./ServerRec.log";*/
#define  MAX_QUEUE 1024

#ifndef _SERVER_H__
#define _SERVER_H__
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
/*------------------function declaration---------------*/
/*read thread function*/
int readTaskThread();
/*write thread function*/
int writeTaskThread();
/*log record thread function*/
int logRecThread();
/*thread create function*/
int createThread(pthread_t px);
/*set comm-type to non-blocking*/
int setSocketNoBlocking(int fd);
/*initial server config params*/
int initServer(int* socket, int port);
/*display system fatal error*/
void sysFatalErr(const char* message, int ret);
#endif
