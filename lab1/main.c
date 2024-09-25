#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <wait.h>
#include <math.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <bits/socket_type.h>
#include <string.h>
#include <semaphore.h>
/*
Задание: требуется написать, отладить и продемонстрировать преподавателю систему из нескольких независимых процессов, совместно решающих вычисляющих гипотенузу по двум катетам (M). Процессы или потоки должны отображать на экране ход своего выполнения в виде отладочных сообщений. Система должны состоять из:
 - главного процесса или потока, принимающего с клавиатуры исходные данные и выводящего на экран результат;
 - нескольких служебных процессов или потоков, способных по отдельности выполнять элементарные арифметические действия - сложение, вычитание, умножение, деление, вычисление квадратного корня и т.п.
 */

//man unix 7

//независимые процессы;
//семафоры;
//сокеты;
//гипотенуза по двум катетам
// порядок выставлять через семафоры

#define SqrSockName  "/tmp/oaSqrFn.sock"
#define SumSockName  "/tmp/oaSumFn.sock"
#define SqrtSockName "/tmp/oaSqrtFn.sock"

typedef double(fn1)(double);
typedef double(fn2)(double,double);


double
    A,
    B,
    A2,
    B2,
    ApB,
    result;
sem_t
    A2_sem,
    B2_sem,
    ApB_sem,
    result_sem;

pid_t  asyncSqr   ();
pid_t  asyncSum   ();
pid_t  asyncSqrt  ();


struct sockaddr_un name;
int ret;
int initIOServe(char* sname);

int main() {

    printf("Введите A: ");
    assert(scanf("%lf", &A));
    printf("Введите B: ");
    assert(scanf("%lf", &B));

    pid_t sqrPid  = fork();
    if (!sqrPid)
    {
        asyncSqr();
        return 0;
    }
    pid_t sumPid  = fork();
    if (!sumPid)
    {
        asyncSum();
        return 0;
    }
    pid_t sqrtPid = fork();
    if (!sqrtPid)
    {
        asyncSqrt();
        return 0;
    }



    return 0;
}

int initIOServe(char* sname) {
    int connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1) {
        fprintf(stderr, "PID %d:\tsock == -1\n", getpid());
        return -1;
    }
    memset(&name, 0, sizeof(name));
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, sname, sizeof(name.sun_path) - 1);
    ret = bind(connection_socket, (const struct sockaddr *) &name, sizeof(name));
    if (ret == -1) {
        fprintf(stderr, "PID %d:\tbind == -1\n", getpid());
        return -1;
    }
    ret = listen(connection_socket, 20);
    if (ret == -1) {
        fprintf(stderr, "PID %d:\tlisten == -1\n", getpid());
        return -1;
    }
    return connection_socket;

};

pid_t  asyncSqr  () {
    int connection_socket = initIOServe(SqrSockName);

    /* Основной цикл обработки подключений. */
    while (1) {

        /* Ожидание входящих подключений. */

        int data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1) {
            fprintf(stderr, "PID %d:\taccept == -1\n", getpid());
            return -1;
        }
        double rx, tx;
        read(data_socket, &rx, sizeof(rx));
        write(data_socket, &tx, sizeof(tx));
        close(data_socket);
    }
}

pid_t  asyncSum   () {
    int connection_socket = initIOServe(SumSockName);
    /* Основной цикл обработки подключений. */
    while (1) {

        /* Ожидание входящих подключений. */

        int data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1) {
            fprintf(stderr, "PID %d:\taccept == -1\n", getpid());
            return -1;
        }
        double rx[2], tx;
        read(data_socket, &rx[0], sizeof(rx));
        write(data_socket, &tx, sizeof(tx));
        close(data_socket);
    }
}

pid_t  asyncSqrt  () {
    int connection_socket = initIOServe(SqrtSockName);
    /* Основной цикл обработки подключений. */
    while (1) {

        /* Ожидание входящих подключений. */

        int data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1) {
            fprintf(stderr, "PID %d:\taccept == -1\n", getpid());
            return -1;
        }
        double rx, tx;
        read(data_socket, &rx, sizeof(rx));
        write(data_socket, &tx, sizeof(tx));

        close(data_socket);
    }
}
