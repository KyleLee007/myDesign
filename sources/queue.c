/*
*  This is a Queue implement C source file.
*       author: KyleLee
*       date:   2014.3.5.
*/
#include "../headers/queue.h"
#include <stdio.h>
#include <stdlib.h>

int initQueue(TaskQueue *pQue)
{
    pQue->front = pQue->rear = (QNodePtr)malloc(sizeof(QNode)); //this is a header node.
    if (! pQue->front)
    {
        printf("Err: malloc Error. Location:queue.c-initQueue.\n");
        return NG;
    }
    pQue->length= 0;
    return OK;
}

int enQueue(TaskQueue *pQue, int elem)
{
    QNodePtr newNode = (QNodePtr)malloc(sizeof(QNode));
    if (!newNode)
    {
        printf("Err: malloc Error. Location:queue.c-enQueue.\n");
        return NG;
    }
    newNode->fdNo = elem;
    newNode->next = NULL;

    pQue->rear->next = newNode;
    pQue->rear = newNode;
    pQue->length++;
    return OK;
}

int deQueue(TaskQueue *pQue, int *pElem)
{
    QNodePtr delNode;
    if (OK == isEmpty(*pQue)) // If queue is empty.
    {
        printf("Err: Empty Queue Error. Location:queue.c-deQueue.\n");
        return NG;
    }

    delNode = pQue->front->next;
    *pElem = delNode->fdNo;

    pQue->front->next = delNode->next;
    if (pQue->rear == delNode)
    {
        pQue->rear = pQue->front;
    }
    pQue->length--;

    free(delNode);
    return OK;
}

int isEmpty(TaskQueue que)
{
    int ret = (que.front == que.rear  &&
                       0 == que.length
                )   ? OK : NG ;
    return ret;
}