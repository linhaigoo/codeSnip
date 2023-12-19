#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <thread>

typedef struct
{
    long type;
    char data[32];
} msg_st;

int main()
{
    msg_st msg;

    int shm_key = ftok("../", 1234);
    int shm_id = shmget(0, 4096, IPC_CREAT | 0604);

    if (shm_id == -1)
    {
        printf("shmget error\n");
        return -1;
    }

    {
        std::ofstream f("shm_id.txt");
        f << shm_id;
        f.close();
    }

    printf("shm_id %d\n", shm_id);

    char *str = (char *)shmat(shm_id, nullptr, 0);

    strcpy(str, "shn content\n");

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100s);

    struct shmid_ds buf;
    shmctl(shm_id, IPC_STAT, &buf);
    printf("buf.shm_nattch %d\n", buf.shm_nattch);
}