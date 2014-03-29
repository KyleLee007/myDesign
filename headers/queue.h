/*
* this is a Queue DS define.
*	author: KyleLee.
*	date: 2014.3.5.
*/
#ifndef _QUEUE_H__
#define _QUEUE_H__
/**********enum type define**********************/
typedef enum BOOL{
	FALSE,
		TRUE
} BOOL;
/***********data structure daclaration*************/
/*single node define*/
#include <pthread.h>

typedef struct QNode{
	int fdNo;
	struct QNode *next;
}QNode, *QNodePtr;

/*Queue collection.*/
typedef struct TaskQueue{
	QNodePtr front; /*Queue's header.*/
	QNodePtr rear; /*Queue's tail.*/
	int length;     /*Queue's length.*/
}TaskQueue;

typedef struct TaskParams{
	/*task queue.*/
	TaskQueue taskQue;
	/*threads' condition var.*/
	pthread_cond_t  cond;
	/*mutex.*/
	pthread_mutex_t  mutex;
}TaskParams;

/************function declaration***************/
 char* initQueue(TaskParams *tp);/*initial Queue.*/
 BOOL enQueue(TaskParams *tp, int elem);/*enter Queue.*/
 BOOL deQueue(TaskParams * tp, int* pElem);/*depart Queue.*/
 BOOL isEmpty(TaskQueue pQue); /*whether the Queue is empty.*/
#endif