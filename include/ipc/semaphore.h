#pragma once
#include <sys/ipc.h>
#include <sys/sem.h>

void semaphone()
{
    key_t key = ftok("/home/chris/Projects/cpp/concurrency/data/sem_file", 'C');
    int semid = semget(key, 5, 0666 | IPC_CREAT);


    struct sembuf abuf = {

    };
}