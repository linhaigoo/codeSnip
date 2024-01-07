#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_PORT_KEY_MAGIC_STR "X"
#define SHM_PORT_ACK_MSGQ_MAGIC_STR "Xpub"

#define MAX_SUBSCRIBER_NUM 20

const char *shm_key_port_magic = SHM_PORT_KEY_MAGIC_STR;

struct ipc_port
{
    union {
        uint8_t pub_id;
        uint8_t sub_id;
        uint8_t magic;
        uint8_t check;
    } attr;
    uint32_t key;
};

typedef struct
{
    int shm_key;
    int pub_id;
    int sub_id;
    int cahce_num;
    uint64_t qos;
} ipc_port_info_t;

typedef struct
{
    long type;
    int size;
    uint8_t data[0];
} ipc_port_msg_t;

struct publish_mgr
{
    int subscriber_cnt;
    int sbuffer_own_cnt[MAX_SUBSCRIBER_NUM];
    int sbuffer_own_required[MAX_SUBSCRIBER_NUM];
};

int subscriber_open_shm_port(int sub_id, int pub_id, int cache_num)
{
    struct ipc_port ipc_key;

    ipc_key.key = 0;
    ipc_key.attr.magic = *(const uint32_t *)(shm_key_port_magic);
    ipc_key.attr.pub_id = pub_id;
    ipc_key.attr.sub_id = 0;
    ipc_key.attr.check = 0;

    int shmid_sub = -1, shmid_ack = -1;
    int ret = 0;

    ipc_port_msg_t *conn_msg = NULL;

    0x1000
    

    do
    {
        shmid_sub = shmget(ipc_key.key, 4096, 0666);
        if (shmid_sub < 0)
        {
            printf("no valid publisher\n");
            ret = -1;
        }

        ipc_key.attr.sub_id = sub_id;

        shmid_ack = shmget(ipc_key.key, 4096, IPC_CREAT | IPC_EXCL | 0666);
        if (shmid_ack < 0)
        {
            printf("may previous node didn't clean all\n");
            ret = -2;
        }

        const char *msg_key_magic = SHM_PORT_ACK_MSGQ_MAGIC_STR;
        int msgq_conn_key = *(const uint32_t *)(msg_key_magic) << 24 + ipc_key.attr.pub_id;

        int msgq_conn_id = -1;
        if ((msgq_conn_id = msgget((key_t)msgq_conn_key, IPC_CREAT | 0666)) == -1)
        {
            printf("msgq_key_conn error\n");
            ret = -3;
        }

        conn_msg = (ipc_port_msg_t *)calloc(1, sizeof(ipc_port_msg_t) + sizeof(ipc_port_info_t));
        ipc_port_info_t *port_info = (ipc_port_info_t *)&conn_msg->data[0];

        port_info->shm_key = ipc_key.key;
        port_info->pub_id = pub_id;
        port_info->sub_id = sub_id;
        port_info->cahce_num = cache_num;

        const char *msg_type_magic = SHM_PORT_ACK_MSGQ_MAGIC_STR;
        long msgtype = *(const uint32_t *)(msg_type_magic);
        conn_msg->type = msgtype;

        if (msgsnd(msgq_conn_id, &conn_msg, sizeof(conn_msg->size) + conn_msg->size, 0) < 0)
        {
            ret = -4;
            break;
        }
        struct shmid_ds sh_info;
        if (shmctl(shmid_ack, IPC_STAT, &sh_info))
        {
            ret = -5;
            break;
        }
        sh_info.shm_perm

    } while (0);

    if (ret < 0)
    {
    }
}

int publisher_open_shm_port(int pub_id)
{
    struct ipc_port ipc_key;
    ipc_key.key = 0;
    ipc_key.attr.pub_id = pub_id;
    ipc_key.attr.sub_id = 0;

    int shm_key_pub = *(const uint32_t *)(shm_key_port_magic) << 24 + ipc_key.key;

    int shmid_pub = 0, shmid_w = 0;
    shmid_pub = shmget(shm_key_pub, 4096, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid_pub < 0)
    {
        printf("no valid publisher\n");
    }
}

int publisher_listen()
{
}