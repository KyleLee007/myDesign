/*
*  This is a Queue implement C source file.
*       author: KyleLee
*       date:   2014.3.5.
*/
#include <stdio.h>
#include <stdlib.h>
#include "../headers/queue.h"

 char* initQueue(TaskParams *tp)
{
	 TaskQueue *pQue = &(tp->taskQue);
	 pthread_mutex_t *pmutex = &(tp->mutex);
	 pthread_cond_t * pcond = &(tp->cond);
	/*--------------------initial Queue.-----------------*/
	 /*this is a header node.*/
    pQue->front = pQue->rear = (QNodePtr)malloc(sizeof(QNode));
    if (! pQue->front)
    {
        return "memory alloc error.";
    }
    pQue->length= 0;
/*-------------------initial mutex.----------------------*/
	/*Init r&w thread mutexs with default attribute.*/
	if ( pthread_mutex_init(pmutex, NULL) )
	{
		 return "taskQue initial error.";
	}
/*------------------initial cond var.----------------------*/	
	/*Init r&w cond-vars with default attribute.*/
	if ( pthread_cond_init(pcond,  NULL) )
	{
		return "cond vars initial error.";
	}
    return NULL;
}

 BOOL enQueue(TaskParams *tp, int elem)
{
	 TaskQueue *pQue = &(tp->taskQue);
	 pthread_mutex_t *pmutex = &(tp->mutex);
	 pthread_cond_t *pcond = &(tp->cond);

	 QNodePtr newNode = (QNodePtr)malloc(sizeof(QNode));
     if (!newNode)
	 {
        printf("Err: malloc Error. Location:queue.c-enQueue.\n");
        return FALSE;
	 }

	/*lock the task-queue.*/
	pthread_mutex_lock(pmutex);

    newNode->fdNo = elem;
    newNode->next = NULL;
    
	pQue->rear->next = newNode;
    pQue->rear = newNode;
    pQue->length++;

	/*unlock the queue.*/
	pthread_mutex_unlock(pmutex);
	/*wake a thread which is waitting forcond up. */
	pthread_cond_signal(pcond);
    return TRUE;
}

 BOOL deQueue(TaskParams *tp, int *pElem)
{
	 TaskQueue *pQue = &(tp->taskQue);
	 pthread_mutex_t *pmutex = &(tp->mutex);
	 pthread_cond_t *pcond = &(tp->cond);

    QNodePtr delNode;

	/*To avoid other threads use queue in the
	  same time. lock the task-queue.*/
	pthread_mutex_lock(pmutex);
	/*If the queue is empty, then wait.*/
	while (isEmpty(*pQue))
	{
		pthread_cond_wait(pcond, pmutex);
	}
	
    delNode = pQue->front->next;
    *pElem = delNode->fdNo;

    pQue->front->next = delNode->next;
    if (pQue->rear == delNode)
    {
        pQue->rear = pQue->front;
    }
    pQue->length--;
	/*unlock the queue.*/
	pthread_mutex_unlock(pmutex);

    free(delNode);
    return TRUE;
}

 BOOL isEmpty(TaskQueue que)
{
     BOOL ret = (que.front == que.rear  &&
                       0 == que.length
                )   ? TRUE : FALSE ;
    return ret;
}