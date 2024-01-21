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
#define __MAX_BUFFER_ALLOC__ 16
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

typedef struct
{
    int shm_id;
    int size;
    int capacity;
    void *shm_data;
} dds_data_t;

struct broadcast_pub
{
    char name[__PUB_NAME_SIZE__];
    std::atomic<int> version;
    dds_data_t fds[__MAX_BUFFER_ALLOC__];
    int head;
    int tail;
    int msg_type_id;
};

typedef struct
{
    char name[__PUB_NAME_SIZE__];
    int domain_id;
    int node_id;
    int ipc_id;
    int msg_type_id;
    int is_pub_sub;
    int cache_num;
    uint64_t qos;
} node_attr_t;

struct pub_node
{
    node_attr_t attr;
    struct broadcast_pub *pub_topic;
    int sub_cnt;
    sub_buffer_mgrs_t *psub_mgrs[__MAX_BUFFER_ALLOC__];
    int pub_id;
};

typedef struct
{
    int sub_max_buffer;
    int nbuffer;
    int buffer[__MAX_BUFFER_TRANSFER_ONCE__];
} sub_buffer_mgrs_t;

struct sub_node
{
    node_attr_t attr;
    bool is_connected;
    struct broadcast_pub *sub_topic;
    sub_buffer_mgrs_t *buffer_mgr;
};

#define DDS_MSG_KEY_CORE_MAGIC_STR "DDSC"
#define DDS_MSG_KEY_NODE_MAGIC_STR "DDSN"

typedef struct
{
    long type;
    int size;
    int ret;
    uint8_t data[52];
} ipc_port_msg_t;

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
    node_attr_t attr;
    uint64_t time;
    uint64_t id;
} topic_t;

typedef struct
{
    node_attr_t attr;
    int name_hash;
    int pub_cnt;
} node_mgr_t;

typedef struct
{
    std::atomic<int> own_id;
    uint64_t ts;
    node_mgr_t item[128];
} core_node_map_t;

static_assert(sizeof(core_node_map_t) < 4096);

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

static int __find_core_node_map(core_node_map_t *map, const node_attr_t *node_info)
{
    const int item_array_size = sizeof(map->item) / sizeof(map->item[0]);
    int cur_name_hash = hash((const char *)&node_info->name[0]);
    for (int i = 0; i < item_array_size; i++)
    {
        node_mgr_t *item = &(map->item[i]);
        if (item->name_hash != cur_name_hash)
        {
            continue;
        }
        else if (item->pub_cnt)
        {
            if (strcmp(item->attr.name, node_info->name))
            {
                return i;
            }
        }
    }
    return -1;
}

int register_core_node_map(core_node_map_t *map, const node_attr_t *node_info)
{
    int ret = -1;

    int item_idx = __find_core_node_map(map, node_info);
    bool node_exist = item_idx >= 0;

    if (node_exist)
    {
        node_mgr_t *item = &(map->item[item_idx]);
        item->pub_cnt++;
        ret = 0;
    }
    else
    {
        const int item_array_size = sizeof(map->item) / sizeof(map->item[0]);
        int cur_name_hash = hash((const char *)&node_info->name[0]);

        for (int i = 0; i < item_array_size; i++)
        {
            node_mgr_t *item = &(map->item[i]);
            if (item->pub_cnt == 0)
            {
                item->attr.ipc_id = node_info->ipc_id;
                memcpy(item->attr.name, node_info->name, sizeof(node_info->name));
                item->name_hash = cur_name_hash;
                item->pub_cnt = 1;
                ret = 0;
            }
        }
    }
    return ret;
}

int unregister_core_node_map(core_node_map_t *map, const node_attr_t *node_info)
{
    int ret = -1;
    int item_idx = __find_core_node_map(map, node_info);
    bool node_exist = item_idx >= 0;

    if (node_exist)
    {
        node_mgr_t *item = &(map->item[item_idx]);
        item->pub_cnt--;
        ret = 0;
    }

    return ret;
}

const node_attr_t *find_core_node_map(core_node_map_t *map, const node_attr_t *node_info)
{
    int item_idx = __find_core_node_map(map, node_info);
    if (item_idx < 0)
    {
        return nullptr;
    }
    return &(map->item[item_idx].attr);
}

int printf_core_node_map(core_node_map_t *map)
{
    const int item_array_size = sizeof(map->item) / sizeof(map->item[0]);
    for (int i = 0; i < item_array_size; i++)
    {
        node_mgr_t *item = &(map->item[i]);
        if (item->pub_cnt)
        {
            printf("#####\nipc_id: %d \nname: %s\n name_hash: %d\n, pub_cnt: %d\n#####\n", item->topic.attr.ipc_id,
                   item->attr.name, item->name_hash, item->pub_cnt);
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

int open_core_node_loop(int domain_id)
{
    signal(SIGTERM, signal_handler);
    int ret = 0;
    int msgq_r_id = -1;
    int msgq_s_id = -1;
    int shm_core_id = -1;
    int cur_id = getpid();

    const char *msg_key_magic = DDS_MSG_KEY_CORE_MAGIC_STR;
    int msgq_r_key = *(const uint32_t *)(msg_key_magic) + (domain_id << 1) + 1;           // domain: node
    int msgq_s_key = *(const uint32_t *)(msg_key_magic) + (domain_id << 1) + 0;           // domain: node
    int shm_map_key = *(const uint32_t *)(DDS_MSG_KEY_CORE_MAGIC_STR) + (domain_id << 1); // domain: core node

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

        core_node_map_t *node_map = (core_node_map_t *)shmat(shm_core_id, nullptr, 0);
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
                    const node_attr_t *node_info = (const node_attr_t *)req->data;
                    int ret = register_core_node_map(node_map, node_info);

                    ipc_port_msg_t ack_msg;
                    ack_msg.ret = ret;
                    core_msg_t *ack_req = (core_msg_t *)&ack_msg.data;
                    ack_req->req_type = IPC_CORE_ACK_PUB;
                    ack_msg.type = node_info->ipc_id;
                    msgsnd(msgq_s_id, &ack_msg, sizeof(ack_msg.data) + sizeof(ack_msg.size), IPC_NOWAIT);
                }
                break;

                case IPC_CORE_REQ_PUB_RM: {
                    const node_attr_t *node_info = (const node_attr_t *)req->data;
                    int ret = unregister_core_node_map(node_map, node_info);

                    ipc_port_msg_t ack_msg;
                    ack_msg.ret = ret;
                    core_msg_t *ack_req = (core_msg_t *)&ack_msg.data;
                    ack_req->req_type = IPC_CORE_ACK_PUB;
                    ack_msg.type = node_info->ipc_id;
                    msgsnd(msgq_s_id, &ack_msg, sizeof(ack_msg.data) + sizeof(ack_msg.size), IPC_NOWAIT);
                }
                break;

                case IPC_CORE_REQ_SUB: {
                    const node_attr_t *node_info = (const node_attr_t *)req->data;
                    const node_attr_t *find_node = find_core_node_map(node_map, node_info);

                    ipc_port_msg_t ack_msg;
                    if (find_node)
                    {
                        ack_msg.ret = 0;
                        core_msg_t *ack_req = (core_msg_t *)&ack_msg.data;
                        ack_req->size = sizeof(topic_t);
                        ack_req->req_type = IPC_CORE_ACK_SUB;
                        node_attr_t *node_info = (node_attr_t *)ack_req->data;
                        memcpy(node_info, find_node, sizeof(topic_t));

                        ack_msg.size = sizeof(ack_msg.size) + ack_req->size;
                        ack_msg.type = node_info->ipc_id;
                    }
                    else
                    {
                        ack_msg.ret = -1;
                    }
                    msgsnd(msgq_s_id, &ack_msg, sizeof(ack_msg.data) + sizeof(ack_msg.size), IPC_NOWAIT);
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

struct pub_node *open_pub_node(int domain_id, const char *name, int cache_num)
{
    int cur_id = getpid();
    int msgq_s_id = -1;
    int msgq_r_id = -1;

    const char *msg_key_magic = DDS_MSG_KEY_CORE_MAGIC_STR;
    int msgq_r_key = *(const uint32_t *)(msg_key_magic) + (domain_id << 1) + 0; // domain: node
    int msgq_s_key = *(const uint32_t *)(msg_key_magic) + (domain_id << 1) + 1; // domain: node
    int shm_id = -1;

    int ret = 0;
    struct pub_node *node = (struct pub_node *)calloc(1, sizeof(struct pub_node));
    if (!node)
    {
        return nullptr;
    }

    strncpy(node->attr.name, name, sizeof(node->attr.name));
    node->attr.domain_id = domain_id;
    node->attr.cache_num = cache_num;

    do
    {
        msgq_s_id = msgget((key_t)msgq_s_key, 0666);
        msgq_r_id = msgget((key_t)msgq_s_key, 0666);
        if ((msgq_s_id == -1) || (msgq_r_id == -1))
        {
            printf("msgq_key_conn error\n");
            ret = -1;
            break;
        }
        shm_id = shmget(0, 4096, IPC_CREAT | 0604);

        if (shm_id == -1)
        {
            break;
        }

        struct broadcast_pub *broacast = (struct broadcast_pub *)shmat(shm_id, nullptr, 0);
        memset(broacast, 0, sizeof(struct broadcast_pub));

        strncpy(broacast->name, node->attr.name, sizeof(broacast->name));

        node->attr.ipc_id = shm_id;
        node->pub_topic = broacast;

        ipc_port_msg_t pub_msg;
        core_msg_t *req = (core_msg_t *)&pub_msg.data;
        node_attr_t *node_info = (node_attr_t *)req->data;

        memcpy(node_info, &node->attr, sizeof(node->attr));

        req->req_type = IPC_CORE_REQ_PUB;
        pub_msg.size = sizeof(pub_msg.size) + req->size;
        pub_msg.type = cur_id;

        msgsnd(msgq_s_id, &pub_msg, pub_msg.size, IPC_NOWAIT);

        ipc_port_msg_t ack_msg;

        int try_cnt = 2;
        for (int i = try_cnt; i--; i > 0)
        {
            usleep(100 * 1000);
            if (msgrcv(msgq_r_id, (void *)&ack_msg, sizeof(ack_msg.data) + sizeof(ack_msg.size), cur_id, IPC_NOWAIT) >=
                0)
            {
                if (!ack_msg.ret)
                {
                    printf("may busy error\n");
                    ret = -1;
                }
                break;
            }
        }
    } while (0);

    if (!ret)
    {
        if (node->pub_topic)
        {
            shmdt(node->pub_topic);
        }
        shmctl(shm_id, IPC_RMID, NULL);
        free(node);
        node = nullptr;
    }

    return node;
}

int adjust_push_topic_fd(dds_data_t *fd_dest, const dds_data_t *fd_src)
{
    int ret = 0;
    if (fd_dest->shm_id != fd_src->shm_id)
    {
        shmdt(fd_dest->shm_data);
        shmctl(fd_dest->shm_id, IPC_RMID, NULL);
    }
    *fd_dest = *fd_src;
    return ret;
}

int queue_pub_topic_fd(struct pub_node *node, const dds_data_t *fd)
{
    int ret = -1;

    struct broadcast_pub *broacast = node->pub_topic;

    auto head = broacast->head;
    auto tail = broacast->tail;

    int next_head_index = broacast->head % __MAX_BUFFER_ALLOC__;
    int tail_index = broacast->tail % __MAX_BUFFER_ALLOC__;

    if (next_head_index != tail_index) // else full
    {
        broacast->version++;
        ret = adjust_push_topic_fd(&broacast->fds[head % __MAX_BUFFER_ALLOC__], fd);
        broacast->head++;
        broacast->version++;
    }
    return ret;
}

int deque_pub_topic_fd(struct pub_node *node, dds_data_t *fd)
{
    int ret = -1;
    struct broadcast_pub *broacast = node->pub_topic;

    auto head = broacast->head;
    auto tail = broacast->tail;
    auto next_tail = tail + 1;

    int next_tail_index = (next_tail) % __MAX_BUFFER_ALLOC__;
    int head_index = head % __MAX_BUFFER_ALLOC__;
    if (tail != head) // else empty
    {
        broacast->version++;
        *fd = broacast->fds[(tail) % __MAX_BUFFER_ALLOC__];
        broacast->tail = next_tail;
        broacast->version++;

        ret = 0;
    }
    return ret;
}

struct sub_node *open_sub_node(int domain_id, const char *name, int cache_num)
{
    int cur_id = getpid();
    int msgq_s_id = -1;
    int msgq_r_id = -1;

    const char *msg_key_magic = DDS_MSG_KEY_CORE_MAGIC_STR;
    int msgq_r_key = *(const uint32_t *)(msg_key_magic) + (domain_id << 1) + 0; // domain: node
    int msgq_s_key = *(const uint32_t *)(msg_key_magic) + (domain_id << 1) + 1; // domain: node
    int shm_id = -1;

    int ret = 0;
    struct sub_node *node = (struct sub_node *)calloc(1, sizeof(struct sub_node));
    if (!node)
    {
        return nullptr;
    }

    strncpy(node->attr.name, name, sizeof(node->attr.name));
    node->attr.domain_id = domain_id;
    node->attr.cache_num = cache_num;

    do
    {
        msgq_s_id = msgget((key_t)msgq_s_key, 0666);
        msgq_r_id = msgget((key_t)msgq_s_key, 0666);
        if ((msgq_s_id == -1) || (msgq_r_id == -1))
        {
            printf("msgq_key_conn error\n");
            ret = -1;
            break;
        }

        shm_id = shmget(0, 4096, IPC_CREAT | 0644);
        if (shm_id == -1)
        {
            printf("shmget error\n");
            ret = -2;
            break;
        }
        node->attr.ipc_id = shm_id;
        node->buffer_mgr = (sub_buffer_mgrs_t *)shmat(shm_id, nullptr, 0);

        ipc_port_msg_t sub_msg;
        core_msg_t *req = (core_msg_t *)&sub_msg.data;
        node_attr_t *node_info = (node_attr_t *)req->data;
        memcpy(node_info, &node->attr, sizeof(node->attr));

        req->req_type = IPC_CORE_REQ_SUB;
        sub_msg.size = sizeof(sub_msg.size) + req->size;
        sub_msg.type = cur_id;

        msgsnd(msgq_s_id, &sub_msg, sub_msg.size, IPC_NOWAIT);

        ipc_port_msg_t ack_msg;

        int try_cnt = 2;
        for (int i = try_cnt; i--; i > 0)
        {
            usleep(100 * 1000);
            if (msgrcv(msgq_r_id, (void *)&ack_msg, sizeof(ack_msg.data) + sizeof(ack_msg.size), cur_id, IPC_NOWAIT) >=
                0)
            {
                if (!ack_msg.ret)
                {
                    printf("may busy error\n");
                    ret = -1;
                }
                else
                {
                    core_msg_t *ack_req = (core_msg_t *)&ack_msg.data;
                    const node_attr_t *ack_node_info = (node_attr_t *)req->data;

                    struct broadcast_pub *broacast =
                        (struct broadcast_pub *)shmat(ack_node_info->ipc_id, nullptr, SHM_RDONLY);
                    node->sub_topic = broacast;
                    node->attr.node_id = ack_node_info->node_id;
                    ret = 0;
                }
                break;
            }
        }
    } while (0);

    if (!ret)
    {
        if (node->sub_topic)
        {
            shmdt(node->sub_topic);
        }
        if (node->buffer_mgr)
        {
            shmdt(node->buffer_mgr);
        }
        if (node->attr.ipc_id)
        {
            shmctl(node->attr.ipc_id, IPC_RMID, NULL);
        }
        free(node);
        node = nullptr;
    }
    return node;
}

int pub_node_handle_connect(struct pub_node *node)
{
    ipc_port_msg_t connect_msg;

    const char *msg_key_magic = DDS_MSG_KEY_NODE_MAGIC_STR;

    int ret = -1;
    int msg_id = -1;

    sub_buffer_mgrs_t *psub_mgr = nullptr;
    int pre_sub_cnt = node->sub_cnt;

    do
    {
        msg_id = msgget((key_t)msg_key_magic, 0666);
        if (msg_id == -1)
        {
            break;
        }
        if (msgrcv(msg_id, (void *)&connect_msg, sizeof(connect_msg.data) + sizeof(connect_msg.size),
                   node->attr.msg_type_id, IPC_NOWAIT) >= 0)
        {
            core_msg_t *req = (core_msg_t *)&connect_msg.data;
            node_attr_t *sub_node_info = (node_attr_t *)req->data;

            int sub_cnt = node->sub_cnt;
            if (sub_cnt >= __MAX_BUFFER_ALLOC__)
            {
                break;
            }

            psub_mgr = (sub_buffer_mgrs_t *)shmat(sub_node_info->ipc_id, nullptr, SHM_RDONLY);
            if (!psub_mgr)
            {
                break;
            }
            node->psub_mgrs[sub_cnt++] = psub_mgr;
            node->sub_cnt = sub_cnt;

            ipc_port_msg_t ack_msg;
            core_msg_t *ack_req = (core_msg_t *)&ack_msg.data;
            node_attr_t *ack_node_info = (node_attr_t *)req->data;
            memcpy(ack_node_info, &node->attr, sizeof(node->attr));

            ack_msg.size = sizeof(connect_msg.size) + req->size;
            ack_msg.type = sub_node_info->msg_type_id;

            msgsnd(msg_id, &ack_msg, ack_msg.size, IPC_NOWAIT);

            ret = 0;
        }
    } while (0);

    if (!ret)
    {
        if (psub_mgr)
        {
            shmdt(psub_mgr);
        }
        if (node->sub_cnt != pre_sub_cnt)
        {
            node->sub_cnt--;
        }
    }

    return ret;
}

int sub_node_connect_pub_node(struct sub_node *node)
{
    if (node->is_connected)
    {
        return -1;
    }

    const char *msg_key_magic = DDS_MSG_KEY_NODE_MAGIC_STR;
    int msg_id = -1;
    int ret = -1;

    do
    {
        msg_id = msgget((key_t)msg_key_magic, 0666);
        if (msg_id == -1)
        {
            break;
        }

        ipc_port_msg_t connect_msg;
        core_msg_t *req = (core_msg_t *)&connect_msg.data;
        node_attr_t *node_info = (node_attr_t *)req->data;
        memcpy(node_info, &node->attr, sizeof(node->attr));

        req->req_type = IPC_CORE_REQ_SUB;
        connect_msg.size = sizeof(connect_msg.size) + req->size;
        connect_msg.type = node->sub_topic->msg_type_id;

        msgsnd(msg_id, &connect_msg, connect_msg.size, IPC_NOWAIT);

        ipc_port_msg_t ack_msg;

        int try_cnt = 2;
        for (int i = try_cnt; i--; i > 0)
        {
            usleep(100 * 1000);
            if (msgrcv(msg_id, (void *)&ack_msg, sizeof(ack_msg.data) + sizeof(ack_msg.size), node->attr.msg_type_id,
                       IPC_NOWAIT) >= 0)
            {
                if (!ack_msg.ret)
                {
                    printf("may busy error\n");
                    ret = -1;
                }
                else
                {
                    core_msg_t *ack_req = (core_msg_t *)&ack_msg.data;
                    node_attr_t *ack_node_info = (node_attr_t *)req->data;

                    struct broadcast_pub *broacast =
                        (struct broadcast_pub *)shmat(ack_node_info->ipc_id, nullptr, SHM_RDONLY);
                    node->sub_topic = broacast;
                    ret = 0;
                }
                break;
            }
        }

    } while (0);
}

int dequeue_sub_node(struct sub_node *node, dds_data_t *fd)
{
    int ret = -1;
    // struct broadcast_pub *broacast = node->sub_topic;

    // auto head = broacast->head;
    // auto tail = broacast->tail;
    // auto next_tail = tail + 1;

    // int next_tail_index = (next_tail) % __MAX_BUFFER_ALLOC__;
    // int head_index = head % __MAX_BUFFER_ALLOC__;
    // if (tail != head) // else empty
    // {
    //     broacast->version++;
    //     *fd = broacast->fds[(tail) % __MAX_BUFFER_ALLOC__];
    //     broacast->tail = next_tail;
    //     broacast->version++;

    //     ret = 0;
    // }
    return ret;
}

int queue_sub_node(struct sub_node *node, dds_data_t *fd)
{
    int ret = -1;
    // struct broadcast_pub *broacast = node->sub_topic;

    // auto head = broacast->head;
    // auto tail = broacast->tail;
    // auto next_tail = tail + 1;

    // int next_tail_index = (next_tail) % __MAX_BUFFER_ALLOC__;
    // int head_index = head % __MAX_BUFFER_ALLOC__;
    // if (tail != head) // else empty
    // {
    //     broacast->version++;
    //     *fd = broacast->fds[(tail) % __MAX_BUFFER_ALLOC__];
    //     broacast->tail = next_tail;
    //     broacast->version++;

    //     ret = 0;
    // }
    return ret;
}
