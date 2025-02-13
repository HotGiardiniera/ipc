#pragma once
#include <iostream>
#include <cstdlib>
#include "fcntl.h"
#include "unistd.h"

void file_lock(int argc, char* argv[])
{
    struct flock fl = {
            F_WRLCK,
            SEEK_SET,
            0,
            0
    };
    int fd;

    if((fd = open("/home/chris/Projects/cpp/concurrency/data/file_lock", O_RDWR)) == -1)
    {
        perror("Couldn't open file_lock");
        exit(-1);
    }

    std::cout << "Press <RETURN> to try to get lock: " << std::endl;
    std::cin.get();
    std::cout << "Trying to get lock" << std::endl;

    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl error on lock get");
        exit(-1);
    }
    std::cout << "got lock" << std::endl;
    std::cout << "Press <RETURN> to release lock: " << std::endl;
    std::cin.get();

    fl.l_type = F_ULOCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl error on unlock");
        exit(-1);
    }
    std::cout << "unlocked" << std::endl;
    close(fd);
}
