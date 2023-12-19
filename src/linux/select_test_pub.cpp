#include <algorithm>
#include <condition_variable>
#include <ctype.h>
#include <iostream>
#include <mutex>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <thread>
#include <unistd.h>

#define LOGI_print printf

static void printf_fd_set(const char *label, fd_set *set)
{
    int32_t *data = (int32_t *)set;
    LOGI_print("fd_set %s:\n", label);
    for (int i = 0; i < sizeof(fd_set) / 4; i++)
    {
        LOGI_print("%08jx ", data[i]);
    }
    LOGI_print("\n");
}

#define STDIN 0

int main()
{
    fd_set fds{};
    FD_ZERO(&fds);
    int fd = 0;
    FD_SET(fd, &fds);
    printf_fd_set("fd 0", &fds);

    FD_ZERO(&fds);
    fd = 1;
    FD_SET(fd, &fds);
    printf_fd_set("fd 1", &fds);

    FD_ZERO(&fds);
    fd = -1;
    FD_SET(fd, &fds);
    printf_fd_set("fd -1", &fds);
    if (FD_ISSET(fd, &fds))
    {
        printf("fd -1 selected\n");
    }

    int _stop_pipe_fd[2];
    if (pipe(_stop_pipe_fd) < 0)
    {
        return -1;
    }

    std::mutex mtx;
    bool thread_start = false;
    std::condition_variable thread_cv;

    std::thread test_select([&]() {
        FD_ZERO(&fds);
        // fd = STDIN;
        // FD_SET(fd, &fds);
        FD_SET(_stop_pipe_fd[0], &fds);

        int max_fd = std::max(-1, _stop_pipe_fd[0]);
        int pipe_fd = _stop_pipe_fd[0];

        {
            std::lock_guard<std::mutex> lk(mtx);
            thread_start = true;
        }
        thread_cv.notify_one();

        struct timeval tv = {20, 0};
        printf("select\n");
        int val = select(max_fd + 1, &fds, nullptr, nullptr, &tv);
        printf("selected\n");
        if (val > 0)
        {
            if (FD_ISSET(fd, &fds))
            {
                printf("fd read event\n");
            }
            else if (FD_ISSET(_stop_pipe_fd[0], &fds))
            {
                int fd = _stop_pipe_fd[0];
                char buff[10];
                read(fd, buff, sizeof(buff));
                printf("_stop_pipe_fd read event %s\n", buff);
            }
            else if (FD_ISSET(pipe_fd, &fds))
            {
                char buff[10];
                int ret = read(pipe_fd, buff, sizeof(buff));
                printf("pipe_fd %d read event %s ret %d\n", pipe_fd, buff, ret);
            }
        }
        printf("select ret %d\n", val);
    });

    std::unique_lock<std::mutex> lk(mtx);
    thread_cv.wait(lk, [&] { return thread_start; });

    std::cout << "thread running" << std::endl;
    std::cout << "satrt close pipe" << std::endl;
    ::close(_stop_pipe_fd[1]); // can trigger select exit
    std::cout << "close pipe 1" << std::endl;
    ::close(_stop_pipe_fd[0]); // cannot trigger select exit
    std::cout << "close pipe 0" << std::endl;
    _stop_pipe_fd[1] = _stop_pipe_fd[0] = -1;

    std::cout << "wait thread" << std::endl;
    test_select.join();
    std::cout << "thread joined" << std::endl;
}