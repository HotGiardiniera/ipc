#pragma once
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <string.h>

#define SHM_SIZE 1024  // Segment size of 1k to start

void shm_p1(int argc, char* argv[])
{
    if (argc > 2) {
        fprintf(stderr, "usage: shmdemo [data_to_write]\n");
        exit(1);
    }
    key_t key = ftok("/home/chris/Projects/cpp/concurrency/data/shm_data", 'S');
    if (key == -1)
    {
        perror("Ftok");
        exit(-1);
    }

    int shm_id = shmget(key, 1024, 0644 | IPC_CREAT);  // can always IPC_CREAT as it will just attach to existing
    if (shm_id == -1)
    {
        perror("shmget");
        exit(-1);
    }
    std::cout << shm_id << std::endl;


    char *data = (char *)shmat(shm_id, (void *)0, 0);
    if (data == (char *)-1)
    {
        perror("shamt");
        exit(1);
    }

    if ( argc == 2 )
    {
        std::cout << "Writing to segment " << argv[1] << std::endl;
        strncpy(data, argv[1], SHM_SIZE);
        data[SHM_SIZE-1] = '\0';
    } else {
        std::cout << "Segment has " << data << std::endl;
    }
    int detach = shmdt(data);
    if (detach == -1)
    {
        perror("shmdt");
        exit(-1);
    }



}
