/*
*  This is a Queue implement C source file.
*       author: KyleLee
*       date:   2014.3.5.
*/
#include <stdio.h>
#include <stdlib.h>
#include "../headers/queue.h"

 BOOL initQueue(TaskQueue *pQue)
{
    pQue->front = pQue->rear = (QNodePtr)malloc(sizeof(QNode)); //this is a header node.
    if (! pQue->front)
    {
        printf("Err: malloc Error. Location:queue.c-initQueue.\n");
        return FALSE;
    }
    pQue->length= 0;
    return TRUE;
}

 BOOL enQueue(TaskQueue *pQue, int elem, pthread_mutex_t *pmutex)
{
    QNodePtr newNode = (QNodePtr)malloc(sizeof(QNode));
    if (!newNode)
    {
        printf("Err: malloc Error. Location:queue.c-enQueue.\n");
        return FALSE;
    }
    newNode->fdNo = elem;
    newNode->next = NULL;

	/*lock the task-queue.*/
	pthread_mutex_lock(pmutex);

    pQue->rear->next = newNode;
    pQue->rear = newNode;
    pQue->length++;

	/*unlock the queue.*/
	pthread_mutex_unlock(pmutex);

    return TRUE;
}

 BOOL deQueue(TaskQueue *pQue, int *pElem, pthread_mutex_t *pmutex)
{
    QNodePtr delNode;
    if (TRUE == isEmpty(*pQue)) // If queue is empty.
    {
        printf("Err: Empty Queue Error. Location:queue.c-deQueue.\n");
        return FALSE;
    }

    delNode = pQue->front->next;
    *pElem = delNode->fdNo;

	/*To avoid other threads use queue in the
	  same time. lock the task-queue.*/
	pthread_mutex_lock(pmutex);

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