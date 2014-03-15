/*
main.c
*/
#include <stdio.h>
#include"../headers/server.h"

int main(void)
{
	int ret=-1;
	int port =8006;
	printf("Server start up!\n");
	/*server main process function.*/
	ret = serverProcess(port);

	if (TRUE == ret)
		printf("Server shutdown normally.(CODE:%d).\n", ret);
	else
		printf("Server abort.(CODE:%d).\n ", ret);
	return ret;
}
