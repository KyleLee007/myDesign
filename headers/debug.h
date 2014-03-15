/*
*   for debug.
*
*/
#define pthread_t long
#define pthread_mutex_t int
#define pthread_cond_t int
#define F_GETFL 1
#define F_SETFL 2
#define  O_NONBLOCK 0
#define EPOLLIN 0
#define  EPOLL_CTL_ADD 0
#define  EPOLLOUT 0
#define socklen_t int

int fcntl(int a, int b)
{
	return 0;
}

int epoll_create(int a)
{
	return -1;
}

int epoll_ctl();
int  epoll_wait();
void pthread_mutex_lock();
void pthread_mutex_unlock();
void pthread_cond_wait();
void pthread_mutex_init();
void pthread_cond_init();
void pthread_create();
typedef union epoll_data {
	void *ptr;
	int fd;
	int u32;
	int u64;
} epoll_data_t;

struct epoll_event {
	int events;      /* epoll event */
	epoll_data_t data;      /* User data variable */
};
