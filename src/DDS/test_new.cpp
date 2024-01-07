#include <condition_variable>
#include <mutex>
#include <stdint.h>

#include <atomic>
#include <signal.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define __MAX_BUFFER_TRANSFER_ONCE__ 4
#define __MAX_BUFFER_ALLOC__ 12
#define __PUB_NAME_SIZE__ 32
#define __MAX_SUB_NODE__ 4

enum _DDS_OPERATION_
{
    CONN,
    SUB,
    SUB_ACK,
    RELEASE,
    RELEASE_ACK,
    DIS_CONN,
};

struct broadcast_pub
{
    char name[__PUB_NAME_SIZE__];
    int version;
    int fd[__MAX_BUFFER_ALLOC__];
    int head;
    int tail;
};

struct _DDS_sync_
{
    /* data */
};

struct _DDS_OP_MSG_
{
    int version; // odd means modifing
    enum _DDS_OPERATION_ op;
    int pid;
    int nbuffer;
    int buffer[__MAX_BUFFER_TRANSFER_ONCE__];
    int cnt;
};

struct pub_node
{
    struct broadcast_pub *pub_topic;
    struct _DDS_sync_ *_notify;
    int sub_cnt;
    const struct _DDS_OP_MSG_ *pub_msg[__MAX_SUB_NODE__];
    struct _DDS_OP_MSG_ *sub_msg[__MAX_SUB_NODE__];
    int buffer_ref_cnt[__MAX_BUFFER_ALLOC__];
    int pub_id;
};

struct sub_node
{
    struct broadcast_pub *sub_topic;
    struct _DDS_sync_ *_wait;
    const struct _DDS_OP_MSG_ *pub_msg;
    struct _DDS_OP_MSG_ *sub_msg;

    int sub_max_buffer;
    int nbuffer;
    int buffer[__MAX_BUFFER_TRANSFER_ONCE__];
};

#define DDS_MSG_KEY_MAGIC_STR "DDSM"
#define DDS_MSG_TYPE_MAGIC_STR "Xcore"

typedef struct
{
    long type;
    int size;
    uint8_t data[56];
} ipc_port_msg_t;

typedef struct
{
    int domain_id;
    int node_id;
    char nodename[32];
    int shm_id;
    int cahce_num;
    int pid;
    uint64_t qos;
} ipc_port_info_t;

enum IPC_CORE_TYPE : uint32_t
{
    IPC_CORE_REQ_PUB,
    IPC_CORE_REQ_PUB_RM,
    IPC_CORE_REQ_SUB,
    IPC_CORE_ACK_PUB,
    IPC_CORE_ACK_SUB,
    IPC_CORE_REQ_RM,
};

typedef struct
{
    int req_type;
    int size;
    uint8_t data[0];
} core_msg_t;

typedef struct
{
    int ipc_id;
    int name_hash;
    char name[28];
    int pub_cnt;
} topic_t;

typedef struct
{
    std::atomic<int> own_id;
    uint64_t ts;
    topic_t item[128];
} core_topic_map_t;

static_assert(sizeof(core_topic_map_t) < 4096);

static bool quit = false;

static void signal_handler(int signal_number)
{
    quit = true;
}

uint32_t hash(const char *str)
{
    uint32_t hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static int __find_core_node_map(core_topic_map_t *map, const topic_t *topic)
{
    const int item_array_size = sizeof(map->item) / sizeof(map->item[0]);
    int cur_name_hash = hash((const char *)&topic->name[0]);
    for (int i = 0; i < item_array_size; i++)
    {
        topic_t *item = &(map->item[i]);
        if (item->name_hash != cur_name_hash)
        {
            continue;
        }
        else if (item->pub_cnt)
        {
            if (strcmp(item->name, topic->name))
            {
                return i;
            }
        }
    }
    return -1;
}

int register_core_node_map(core_topic_map_t *map, const topic_t *topic)
{
    int ret = -1;

    int item_idx = __find_core_node_map(map, topic);
    bool topic_exist = item_idx >= 0;

    if (topic_exist)
    {
        topic_t *item = &(map->item[item_idx]);
        item->pub_cnt++;
        ret = 0;
    }
    else
    {
        const int item_array_size = sizeof(map->item) / sizeof(map->item[0]);
        int cur_name_hash = hash((const char *)&topic->name[0]);

        for (int i = 0; i < item_array_size; i++)
        {
            topic_t *item = &(map->item[i]);
            if (item->pub_cnt == 0)
            {
                item->ipc_id = topic->ipc_id;
                memcpy(item->name, topic->name, sizeof(topic->name));
                item->name_hash = cur_name_hash;
                item->pub_cnt = 1;
                ret = 0;
            }
        }
    }
    return ret;
}

int unregister_core_node_map(core_topic_map_t *map, const topic_t *topic)
{
    int ret = -1;
    int item_idx = __find_core_node_map(map, topic);
    bool topic_exist = item_idx >= 0;

    if (topic_exist)
    {
        topic_t *item = &(map->item[item_idx]);
        item->pub_cnt--;
        ret = 0;
    }

    return ret;
}

const topic_t *find_core_node_map(core_topic_map_t *map, const topic_t *topic)
{
    int item_idx = __find_core_node_map(map, topic);
    if (item_idx < 0)
    {
        return nullptr;
    }
    return &(map->item[item_idx]);
}

int printf_core_node_map(core_topic_map_t *map)
{
    const int item_array_size = sizeof(map->item) / sizeof(map->item[0]);
    for (int i = 0; i < item_array_size; i++)
    {
        topic_t *item = &(map->item[i]);
        if (item->pub_cnt)
        {
            printf("#####\nipc_id: %d \nname: %s\n name_hash: %d\n, pub_cnt: %d\n#####\n", item->ipc_id, item->name,
                   item->name_hash, item->pub_cnt);
        }
    }
}

typedef struct
{
    int total_pub_cnt;
    int total_sub_cnt;
    int pub_err;
    int sub_err;
} core_stat_t;

static core_stat_t core_stat;

int open_core_node()
{
    signal(SIGTERM, signal_handler);
    int ret = 0;
    int msgq_r_id = -1;
    int msgq_s_id = -1;
    int shm_core_id = -1;
    int cur_id = getpid();

    const char *msg_key_magic = DDS_MSG_KEY_MAGIC_STR;
    int msgq_r_key = *(const uint32_t *)(msg_key_magic) + 1;
    int msgq_s_key = *(const uint32_t *)(msg_key_magic) + 0;
    int shm_map_key = *(const uint32_t *)(DDS_MSG_KEY_MAGIC_STR);

    do
    {
        msgq_r_id = msgget((key_t)msgq_r_key, IPC_CREAT | 0666);
        msgq_s_id = msgget((key_t)msgq_s_key, IPC_CREAT | 0666);

        if ((msgq_s_id == -1) || (msgq_r_id == -1))
        {
            printf("core node msgq construct error\n");
            ret = -1;
            break;
        }

        shm_core_id = shmget(shm_map_key, 4096, IPC_CREAT | 0604);

        struct shmid_ds buf;
        shmctl(shm_core_id, IPC_STAT, &buf);

        if (buf.shm_nattch > 1)
        {
            printf("there may be %d corenode exist\n", buf.shm_nattch);
        }

        core_topic_map_t *node_map = (core_topic_map_t *)shmat(shm_core_id, nullptr, 0);
        int pre_own_id = node_map->own_id.load();

        if (!node_map->own_id.compare_exchange_strong(pre_own_id, cur_id))
        {
            int cur_own_id = node_map->own_id.load();
            printf("pid %d own the core node\n", cur_own_id);
            break;
        }
        else
        {
            if (pre_own_id != 0)
            {
                kill(pre_own_id, SIGTERM);
            }
        }

        usleep(100 * 1000);
        shmctl(shm_core_id, IPC_STAT, &buf);
        if (buf.shm_nattch > 1)
        {
            printf("fatal error : there may be %d core node exist\n", buf.shm_nattch);
        }

        while (!quit)
        {
            // listening
            ipc_port_msg_t r_msg;
            if (msgrcv(msgq_r_id, (void *)&r_msg, sizeof(r_msg.data) + sizeof(r_msg.size), 0, 0) < 0) // wait an event
            {
                const core_msg_t *req = (const core_msg_t *)&r_msg.data;
                switch (req->req_type)
                {
                case IPC_CORE_REQ_PUB: {
                    const topic_t *topic = (const topic_t *)req->data;
                    register_core_node_map(node_map, topic);
                }
                break;

                case IPC_CORE_REQ_PUB_RM: {
                    const topic_t *topic = (const topic_t *)req->data;
                    unregister_core_node_map(node_map, topic);
                }
                break;

                case IPC_CORE_REQ_SUB: {
                    const topic_t *topic = (const topic_t *)req->data;
                    const topic_t *find_topic = find_core_node_map(node_map, topic);
                    if (find_topic)
                    {
                        ipc_port_msg_t ack_msg;
                        core_msg_t *ack_req = (core_msg_t *)&ack_msg.data;
                        ack_req->size = sizeof(topic_t);
                        ack_req->req_type = IPC_CORE_ACK_SUB;
                        topic_t *ack_topic = (topic_t *)req->data;
                        memcpy(ack_topic, find_topic, sizeof(topic_t));

                        ack_msg.size = sizeof(ipc_port_msg_t) + ack_req->size;
                        ack_msg.type = topic->ipc_id;
                        msgsnd(msgq_s_id, &ack_msg, sizeof(ack_msg.data) + sizeof(ack_msg.size), IPC_NOWAIT);
                    }
                }
                break;

                default:
                    break;
                }
            }
        }

    } while (0);

    if (msgq_r_id != -1)
    {
        msgctl(msgq_r_id, IPC_RMID, NULL);
    }
    if (msgq_s_id != -1)
    {
        msgctl(msgq_s_id, IPC_RMID, NULL);
    }
    // don't remove shm_core_id
}

struct pub_node *open_pub_node(const char *name, int domain_id)
{
    int msgq_id = -1;

    const char *msg_key_magic = DDS_MSG_KEY_MAGIC_STR;
    int msgq_r_key = *(const uint32_t *)(msg_key_magic) + 0;
    int msgq_s_key = *(const uint32_t *)(msg_key_magic) + 1;
    ipc_port_msg_t *pub_msg = nullptr;

    int ret = 0;
    do
    {
        if ((msgq_id = msgget((key_t)msgq_s_key, 0666)) == -1)
        {
            printf("msgq_key_conn error\n");
            ret = -1;
            break;
        }

        pub_msg = (ipc_port_msg_t *)calloc(1, sizeof(ipc_port_msg_t) + sizeof(ipc_port_info_t));
        ipc_port_info_t *port_info = (ipc_port_info_t *)&pub_msg->data[0];
        const char *msg_type_magic = DDS_MSG_TYPE_MAGIC_STR;
        long msgtype = *(const uint32_t *)(msg_type_magic);
        pub_msg->type = msgtype;

        strncpy(port_info->nodename, name, sizeof(port_info->nodename));
        int shm_id = shmget(0, 4096, IPC_CREAT | 0604);

        if (shm_id == -1)
        {
            break;
        }

        struct pub_node *node = (struct pub_node *)shmat(shm_id, nullptr, 0);
        memset(node, 0, sizeof(struct pub_node));

        port_info->domain_id = domain_id;
        port_info->shm_id = shm_id;
        port_info->node_id = 0;
        port_info->cahce_num = 0;
        port_info->pid = getpid();
        port_info->qos = 0;

        int shm_map_key = *(const uint32_t *)(DDS_MSG_KEY_MAGIC_STR);
        shm_map_id = shmget(shm_map_key, 4096, 0666);
        core_topic_map_t *map =
    } while (0);

    return node;
}
