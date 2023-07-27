#define _GNU_SOURCE
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void *sub_thread_func(void *arg)
{
    sleep(1);
    int sub_fd = open("test_sub.txt", O_CREAT | O_RDWR | O_EXCL, 0644);
    int byte_w = write(sub_fd, "12345", 5);
    printf("sub_fd %d\n", sub_fd);
    // close(sub_fd);
}

int main()
{
    int fd = open("test.txt", O_CREAT | O_RDWR | O_EXCL, 0644);
    int byte_w = write(fd, "12345", 5);

    pthread_t sub_pid;
    int ret = pthread_create(&sub_pid, NULL, sub_thread_func, NULL);
    int fd2 = open("test2.txt", O_CREAT | O_RDWR | O_EXCL, 0644);
    int byte_w2 = write(fd2, "12345", 5);
    pthread_join(sub_pid, NULL);
    int fd3 = open("test3.txt", O_CREAT | O_RDWR | O_EXCL, 0644);
    int byte_w3 = write(fd3, "12345", 5);
    printf("fd %d fd2 %d fd3 %d\n", fd, fd2, fd3);

    // int fd = 0;
    // int fd2 = 0;
    // int fd2 = open("test.txt", O_RDWR | O_EXCL);
    // int fd2 = open("test.txt", O_CREAT | O_RDWR);
    // printf("fd %d fd2 %d\n", fd, fd2);
    char filePath[PATH_MAX];
    char linkPath[PATH_MAX];
    snprintf(linkPath, PATH_MAX, "/proc/self/fd/%d", fd);
    printf("linkPath %s\n", linkPath);
    ret = readlink(linkPath, filePath, PATH_MAX);
    if (ret != -1)
    {
        printf("filePath %s %jx\n", filePath, filePath);
        char *filename = basename(filePath);
        printf("filename %jx\n", filename);
        if (filename)
            printf("filename %s %jx", filename, filename);

        // remove(filename);
    }
    printf("ret %d\n", ret);
}
