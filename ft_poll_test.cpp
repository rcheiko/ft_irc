#include<poll.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>

int main(int ac, char **av)
{
    int fd = 0;
    char buffer[256];
    int ret, pret;
    struct pollfd fds[1];
    int timeout;


    while (1)
    {
        fds[0].fd = fd;
        fds[0].events = POLLIN;
        timeout = 5000;
        pret = poll(fds, 1, timeout);
        memset((void*)buffer, 0, 256);
        ret = read(fds[0].fd, (void*)buffer, 10);
        printf("buffer = %s\n", buffer);
    }
    return (0);
}
