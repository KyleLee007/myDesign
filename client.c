#include "headers/h.h"
int main()
{
    int fd;
    struct sockaddr_in dest;
    fd = socket(AF_INET, SOCK_STREAM,0);
    memset(&dest, 0x00, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(8006);
    dest.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(!connect(fd, (struct sockaddr*)&dest, sizeof(dest)))
    {
      //  printf("connected!\n");
        write(fd,"nihao",sizeof("nihao"));
       // write(fd,"wohao",sizeof("wohao"));
    }
    sleep(3);
    close(fd);
}
