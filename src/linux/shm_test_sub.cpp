#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <thread>

inline bool file_exists(const std::string &name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

typedef struct
{
    long type;
    char data[32];
} msg_st;

int main()
{
    while (!file_exists("shm_id.txt"))
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    int shm_id = -1;

    {
        std::ifstream f("shm_id.txt");
        f >> shm_id;
        f.close();
    }

    // int shm_key = ftok("../", 1234);
    // shm_id = shmget(shm_key, 4096, IPC_CREAT | 0000);

    if (shm_id == -1)
    {
        printf("shmget error\n");
        return -1;
    }
    printf("shm_id %d\n", shm_id);

    struct shmid_ds buf;
    shmctl(shm_id, IPC_STAT, &buf);
    printf("permission %o\n", buf.shm_perm.mode);
    // buf.shm_perm.mode = 0400;
    // shmctl(shm_id, IPC_SET, &buf);

    // char *str = (char *)shmat(shm_id, nullptr, 0);
    char *str = (char *)shmat(shm_id, nullptr, SHM_RDONLY);
    if ((long)str != -1)
    {
        printf("str pointer %p \n", str);
        std::cout << "str content: " << str << std::endl;
        // strcpy(str, "shn ro content\n");
    }
    else
    {
        std::cout << " errno " << errno << " stderr(errno) " << strerror(errno) << std::endl;
    }

    printf("shm_id %d\n", shm_id);

    // struct shmid_ds buf;
    shmctl(shm_id, IPC_STAT, &buf);
    printf("buf.shm_nattch %d\n", buf.shm_nattch);
}