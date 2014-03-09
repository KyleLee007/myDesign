/*
* this is a Queue DS define.
*	author: KyleLee.
*	date: 2014.3.5.
*/

/***********data structure daclaration*************/
/*single node define*/
typedef struct QNode{
	int fdNo;
	struct QNode *next;
}QNode, *QNodePtr;

/*Queue collection.*/
typedef struct {
	QNodePtr front; /*Queue's header.*/
	QNodePtr rear; /*Queue's tail.*/
	int length;     /*Queue's length.*/
}TaskQueue;

/**********macro define**********************/
#define NG -1
#define OK 0

/************function declaration***************/
int initQueue(TaskQueue *pQue);/*initial Queue.*/
int enQueue(TaskQueue *pQue, int elem);/*enter Queue.*/
int deQueue(TaskQueue *pQue, int* pElem);/*depart Queue.*/
int isEmpty(TaskQueue pQue); /*is the Queue not empty?*/
