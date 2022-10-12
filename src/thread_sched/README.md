## Linux优先级

## PR

linux PR为-100 ~ +39， PR值越小，优先级越高

normal processes: PR = 20 + NI ( NI 范围: -20-19)

real-time processes: PR=-1-**real_time_priority **(real_time_priority ranges from 1 to 99)

## 设置PR

int pthread_setschedparam (pthread_t \__target_thread, int \__policy, const struct sched_param *__param)

```c++
sch.sched_priority = sched_get_priority_max(SCHED_FIFO);
pthread_setschedparam(thread_1.native_handle(), SCHED_FIFO, &sch)
```

当调度策略使用SCHED_FIFO/SCHED_RR时，设置为real-time processes，sch.sched_priority为real_time_priority，即优先级最高的值对应99，此时终端上top命令的PR列会标记为RT

当调度策略使用SCHED_OTHER时，设置为normal process,且sch.sched_priority必须为0. 此时默认PR为20.

## 设置nice

当进程为real-time processes时，也可以设置nice，但对PR无影响

当进程为normal processes时，可以设置nice，对PR产生影响

```c++
setpriority(PRIO_PROCESS, syscall(SYS_gettid), 10);
```

可以手动实时修改进程的NI值，需要注意当前用户权限和进程的权限，非sudo状态只能增加NI值，不能减小

## 命令行

```
sudo renice -n -19 -p 96894 (不加sudo不能将nice值调小)
nice -n 10 ./src/thread_sched/thread_sched_test
sudo chrt -o -p 0  965738 （-o : --other）
```

绑核

```
sudo taskset --cpu-list 1 nice -n 10 ./src/thread_sched/thread_sched_test
```

## 参考：

https://stackoverflow.com/questions/8887531/which-real-time-priority-is-the-highest-priority-in-linux

https://stackoverflow.com/questions/63674522/linux-sched-fifo-not-respecting-thread-priorities

https://man7.org/linux/man-pages/man1/chrt.1.html
