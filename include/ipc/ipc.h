#pragma once
#include <cstdio>
#include <csignal>
#include <unistd.h>
#include "sys/wait.h"


void sig_handler(int sig)
{
    const char msg[] = "Ahhh! SIGINT!\n";
    write(0, msg, sizeof(msg));

}

void signals()
{
    struct sigaction sa = {};
    sa.sa_handler = sig_handler;
    sa.sa_flags = SA_RESTART;;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Can't set sigaction");
        exit(1);
    }

    char s[200];
    std::cout << "Enter a string: ";
    if (fgets(s, sizeof(s), stdin) == NULL) {
        perror("fgets");
    } else {
        std::cout << "You entered: " << s << std::endl;
    }
}


void pipes()
{
    int pipe_fds[2];
    char buf[30];

    if (pipe(pipe_fds) == -1)
    {
        perror("Couldn't open pipe");
        exit(1);
    }

    if (fork() == 0)
    {
        std::cout << "IN CHILD\n";
        write(pipe_fds[1], "From child", 11);
        std::cout << "Child leaving\n";
        exit(0);

    } else {
        // Assume parent
        std::cout << "IN Parent Reading\n";
        read(pipe_fds[0], buf, 11);
        std::cout << "WE got this: " << buf << std::endl;
        wait(nullptr);
    }

}

#define FIFO_NAME "ipc_fifo"

void fifo_creat()
{
    mknod(FIFO_NAME, S_IFIFO | 0644, 0);
}

int fifo_write()
{
    char s[300];
    int fd;
    size_t num;

    fifo_creat();

    std::cout << "Thread 1: waiting for readers...\n";
    fd = open(FIFO_NAME, O_WRONLY);  // this blocks until there is a reader, because of WRONLY
    std::cout << "Thread 1: got a reader...\n";
    while(1) {
        std::cout << "Enter some text: ";
        std::cin.getline(s, 300);
        if (strcmp(s, "exit") == 0)
        {
            std::cout << "Thread 1 is over this\n";
            break;
        }
        num = write(fd, s, strlen(s));
        if ( num == -1 )
        {
            perror("Thread 1: Couldn't not write to fifo");
        } else {
            std::cout << "Thread 1: Wrote " << s << std::endl;
        }

    }

    close(fd);

    return 0;

}

int fifo_read()
{
    char s[300];
    int fd;
    size_t num;

    // Open the fifo
    fifo_creat();
    std::cout << "Thread 2: waiting for writers...\n";
    fd = open(FIFO_NAME, O_RDONLY);  // this blocks until there is a reader, because of WRONLY
    std::cout << "Thread 2: got a writer...\n";

    do {
        num = read( fd, s, 300 );
        if(num == -1)
        {
            perror("Error Reading");
        } else if (num > 0) {
            s[num] = '\0';
            std::cout << "Thread 2: read " << s << std::endl;
        }

    } while( num > 0);

    return 0;
}

