#define _GNU_SOURCE
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int fd = open("test.txt", O_CREAT | O_RDWR);
    int byte_w = write(fd, "12345", 5);
    // int fd = 0;
    int fd2 = 0;
    // int fd2 = open("test.txt", O_RDWR | O_EXCL);
    // int fd2 = open("test.txt", O_CREAT | O_RDWR);
    printf("fd %d fd2 %d\n", fd, fd2);
    char filePath[PATH_MAX];
    char linkPath[PATH_MAX];
    snprintf(linkPath, PATH_MAX, "/proc/self/fd/%d", fd);
    printf("linkPath %s\n", linkPath);
    int ret = readlink(linkPath, filePath, PATH_MAX);
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
