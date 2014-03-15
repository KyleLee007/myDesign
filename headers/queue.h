/*
* this is a Queue DS define.
*	author: KyleLee.
*	date: 2014.3.5.
*/
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

/************function declaration***************/
 BOOL initQueue(TaskQueue *pQue);/*initial Queue.*/
 BOOL enQueue(TaskQueue *pQue, int elem, pthread_mutex_t *mutex);/*enter Queue.*/
 BOOL deQueue(TaskQueue *pQue, int* pElem, pthread_mutex_t *mutex);/*depart Queue.*/
 BOOL isEmpty(TaskQueue pQue); /*whether the Queue is empty.*/