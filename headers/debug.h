/*
*   for debug.
*
*/
#define pthread_t long
#define pthread_mutex_t int
#define F_GETFL 1
#define F_SETFL 2
#define  O_NONBLOCK 0

int fcntl(int a, int b)
{
	return 0;
}