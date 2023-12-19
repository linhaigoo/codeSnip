#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/msg.h>
#include <thread>

typedef struct
{
    long type;
    char data[32];
} msg_st;

int main()
{
    msg_st msg;

    int msgq_key = ftok("../", 1234);
    // msgq_key = 0;
    int rid = -1;
    if ((rid = msgget((key_t)msgq_key, IPC_CREAT | 0666)) == -1)
    {
        printf("msgq_key_r error\n");
        return -1;
    }
    printf("rid %d\n", rid);
    msg.type = 1234;

    {
        std::ofstream f("msg_id.txt");
        f << rid;
        f.close();
    }

    const char *msg_str = "12345678";

    memcpy(msg.data, msg_str, strlen(msg_str) + 1);

    if (msgsnd(rid, &msg, sizeof(msg.data), IPC_NOWAIT) < 0)
    {
        return -3;
    }
    printf("msgsnd success %s \n", msg.data);
    memset(msg.data, 0, sizeof(msg.data));

    printf("msg.data %s \n", msg.data);

    if (msgrcv(rid, (void *)&msg, sizeof(msg.data), 0, IPC_NOWAIT) < 0)
    {
        std::cout << msg.data << std::endl;
        return -2;
    }
    printf("msgrecv success %s \n", msg.data);
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
}