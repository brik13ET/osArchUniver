#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <math.h>
#include <stdarg.h>
#include <assert.h>
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

#define asyncify(PID,  FN, RET_PTR, args...) \
do { \
    PID = fork(); \
    if (PID == 0) \
        *RET_PTR = FN(args); \
} while(0)

double A, B;

inline pid_t  asyncSqr   (double* in, double* out);
inline pid_t  asyncSum   (double* in, double* out);
inline pid_t  asyncSqrt  (double* in, double* out);
double sum(double a, double b);

int main() {

    printf("Введите A: ");
    assert(scanf("%lf", &A));
    printf("Введите B: ");
    assert(scanf("%lf", &B));
    printf(
            "A*A: %lf\n"
            "B*B: %lf\n"
            "+: %lf\n"
            "sqrt: %lf\n",
            A*A,
            B * B,
            A*A + B * B,
            sqrt(A*A + B * B)

    );

    return 0;
}

inline pid_t asyncSqr   (double* in, double* out)
{
    pid_t ret;
    asyncify(ret, pow, out, *in, 2);
    return ret;
}

inline pid_t asyncSum   (double* in, double* out)
{
    pid_t ret;
    asyncify(ret, sum, out, in[0], in[1]);
    return ret;
}

inline pid_t asyncSqrt  (double* in, double* out)
{
    pid_t ret;
    asyncify(ret, sqrt, out, *in);
    return ret;
}

double sum(double a, double b)
{
    return a + b;
}