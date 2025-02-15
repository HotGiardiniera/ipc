#pragma once
#include <sys/msg.h>
#include <sys/ipc.h>
#include <iostream>


struct msg {
    char msg[20];
};

struct chris_msgbuf {
    long mtype;
    struct msg my_msg;

};

void queue_send()
{
    // Create Queue
    key_t key = ftok("/home/chris/Projects/cpp/concurrency/data/queue_file", 'A');
    if (key == -1)
    {
        perror("ftok");
        exit(-1);
    }
    std::cout << "Key is: " << std::hex << std::showbase <<  key << std::endl;
    // Get queue ID;
    int msg_q_id = msgget(key, 0666 | IPC_CREAT);
    if (msg_q_id == -1)
    {
        perror("MSG_Q invalid");
        exit(-1);
    }
    std::cout << "MSQ Queue ID: " << msg_q_id << std::endl;

    struct chris_msgbuf chris = {2, "Hello chris2\0"};
    int ret = msgsnd(msg_q_id, &chris, sizeof(chris.my_msg), 0);
}

void queue_receive()
{
    // Create Queue
    key_t key = ftok("/home/chris/Projects/cpp/concurrency/data/queue_file", 'A');
    if (key == -1)
    {
        perror("Key invalid");
        exit(-1);
    }
    std::cout << "Key is: " << std::hex << std::showbase << key << std::endl;

    // Get queue ID;
    int msg_q_id = msgget(key, 0666);
    if (msg_q_id == -1)
    {
        perror("MSG_Q invalid");
        exit(-1);
    }
    struct chris_msgbuf chris;
    size_t ret = msgrcv(msg_q_id, &chris, sizeof(chris.my_msg), 2, 0);
    std::cout << "Queue sent: " << chris.my_msg.msg << std::endl;

}
