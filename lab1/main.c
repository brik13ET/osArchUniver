#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h> // O_CREAT
#include <semaphore.h>
#include <sys/socket.h>

#define prompt(T,VT,A) promptFn(T,VT,&A, "Enter "#A": ")
#ifdef DEBUG
#define DEBUG(X,T) printf("[DEBUG] PID=% 5d "#X": "T"\n",getpid(), X)
#endif

double A, B;
pid_t
	pid0,
	pid1,
	pid2;

sem_t
	*pMul1,
	*pMul2,
	*pSum1,
	*pSum2,
	*pSqrt1,
	*pSqrt2;

int my_fds[2];

// pc = Parent to child channel
// cp = Child to parent channel
int sock_sum_pc[2],
	sock_sum_cp[2],
	sock_mul_pc[2],
	sock_mul_cp[2],
	sock_sqrt_pc[2],
	sock_sqrt_cp[2];

int promptFn(char* text, char* valT, void* arg, char* argStr)
{
	if (text == NULL)
		text = argStr;
	printf(text);
	return scanf(valT, arg);
}

void child_mute()
{
	if (my_fds[0]) close(my_fds[0]);
	if (my_fds[1]) close(my_fds[1]);
}

void kill_childs()
{
	printf("\n");
	if (pid0) {
		printf("sumServer kill\n");
		if (kill(pid0, 9))
			printf("kill() returned non-zero");
	}
	if (pid1) {
		printf("mulServer kill\n");
		if (kill(pid1, 9))
			printf("kill() returned non-zero");
	}
	if (pid2){
		printf("sqrtServer kill\n");
		if (kill(pid2, 9))
			printf("kill() returned non-zero");
	}
}

void sync_close()
{
	sem_close(pMul1);
	sem_close(pMul2);
	sem_close(pSum1);
	sem_close(pSum2);
	sem_close(pSqrt1);
	sem_close(pSqrt2);
	sem_unlink("/mul_1");
	sem_unlink("/mul_2");
	sem_unlink("/sum_1");
	sem_unlink("/sum_2");
	sem_unlink("/sqrt_1");
	sem_unlink("/sqrt_2");
}

void ipc_close()
{
	close(sock_sum_pc	[1]);
	close(sock_sum_cp	[0]);
	close(sock_mul_pc	[1]);
	close(sock_mul_cp	[0]);
	close(sock_sqrt_pc	[1]);
	close(sock_sqrt_cp	[0]);
}

void ctrlc()
{
	kill_childs();
	sync_close();
	ipc_close();
	exit(0);
}

void sigpipe()
{
	fflush(stdout);
	fprintf(stderr, "sigpipe: error on PID=%d\n", getpid());
	return;
}

void sumServer()
{
#ifdef DEBUG
	printf("PID=% 5d sumServer started\n", getpid());
#endif
	double input[2];
	while (true)
	{
		sem_wait(pSum1); // Ожидаем данные
		read(sock_sum_pc[0], input, sizeof(input));
		printf("PID=% 5d %lf + %lf = ", getpid(), input[0], input[1]);
		input[0] = input[0] + input[1];
		printf("%lf\n", input[0]);
		fflush(stdout);
		
		write(sock_sum_cp[1], &input[0], sizeof(input[0]));
		sem_post(pSum2);
	}
}

void mulServer()
{
#ifdef DEBUG
	printf("PID=% 5d mulServer started\n", getpid());
#endif
	double input[2];
	while (true)
	{
		sem_wait(pMul1); // Ожидаем данные
		read(sock_mul_pc[0], input, sizeof(input));
		printf("PID=% 5d %lf * %lf = ", getpid(), input[0], input[1]);
		input[0] = input[0] * input[1];
		printf("%lf\n", input[0]);
		fflush(stdout);
		write(sock_mul_cp[1], &input[0], sizeof(input[0]));
		sem_post(pMul2);
	}
}

void sqrtServer()
{
#ifdef DEBUG
	printf("PID=% 5d sqrtServer started\n", getpid());
#endif
	double input[1] = { 0 };
	while (true)
	{
		sem_wait(pSqrt1); // Ожидаем данные
		read(sock_sqrt_pc[0], input, sizeof(input));
		printf("PID=% 5d %lf ** 0.5 = ", getpid(), input[0]);
		input[0] = sqrt(input[0]);
		printf("%lf\n", input[0]);
		fflush(stdout);
		write(sock_sqrt_cp[1], &input[0], sizeof(input[0]));
		sem_post(pSqrt2);
	}
}

int main(int, char**)
{
	signal(SIGPIPE, sigpipe);
	// printf("PID=% 6d main\n", getpid());
#ifdef DEBUG
	DEBUG(main, "%p");
#endif
	

	pMul1 = sem_open("/mul_1", O_CREAT, 0644, 0),
	pMul2 = sem_open("/mul_2", O_CREAT, 0644, 0),
	pSum1 = sem_open("/sum_1", O_CREAT, 0644, 0),
	pSum2 = sem_open("/sum_2", O_CREAT, 0644, 0),
	pSqrt1 = sem_open("/sqrt_1", O_CREAT, 0644, 0),
	pSqrt2 = sem_open("/sqrt_2", O_CREAT, 0644, 0);

	socketpair(AF_UNIX, SOCK_STREAM, 0, sock_sum_pc);
	socketpair(AF_UNIX, SOCK_STREAM, 0, sock_sum_cp);
	pid0 = fork();
	if (pid0 == 0)
	{
		close(sock_sum_pc[1]);
		close(sock_sum_cp[0]);
		my_fds[0] = sock_sum_pc[0];
		my_fds[1] = sock_sum_cp[1];
		signal(9, child_mute);
		sumServer();
		return 0;
	}
	close(sock_sum_pc[0]);
	close(sock_sum_cp[1]);

	socketpair(AF_UNIX, SOCK_STREAM, 0, sock_mul_pc);
	socketpair(AF_UNIX, SOCK_STREAM, 0, sock_mul_cp);
	pid1 = fork();
	if (pid1 == 0)
	{

		close(sock_mul_pc[1]);
		close(sock_mul_cp[0]);
		my_fds[0] = sock_mul_pc[0];
		my_fds[1] = sock_mul_cp[1];
		
		signal(9, child_mute);
		mulServer();
		return 0;
	}
	close(sock_mul_pc[0]);
	close(sock_mul_cp[1]);

	socketpair(AF_UNIX, SOCK_STREAM, 0, sock_sqrt_pc);
	socketpair(AF_UNIX, SOCK_STREAM, 0, sock_sqrt_cp);
	pid2 = fork();
	if (pid2 == 0)
	{
		close(sock_sqrt_pc[1]);
		close(sock_sqrt_cp[0]);
		my_fds[0] = sock_sqrt_pc[0];
		my_fds[1] = sock_sqrt_cp[1];
		
		signal(9, child_mute);
		sqrtServer();
		return 0;
	}
	close(sock_sqrt_pc[0]);
	close(sock_sqrt_cp[1]);

	signal(SIGINT, ctrlc);


	#ifdef DEBUG
		// DEBUG(sock_sum_pc[0], "%d");
		DEBUG(sock_sum_pc[1], "%d");
		DEBUG(sock_sum_cp[0], "%d");
		// DEBUG(sock_sum_cp[1], "%d");
		// DEBUG(sock_mul_pc[0], "%d");
		DEBUG(sock_mul_pc[1], "%d");
		DEBUG(sock_mul_cp[0], "%d");
		// DEBUG(sock_mul_cp[1], "%d");
		// DEBUG(sock_sqrt_pc[0], "%d");
		DEBUG(sock_sqrt_pc[1], "%d");
		DEBUG(sock_sqrt_cp[0], "%d");
		// DEBUG(sock_sqrt_cp[1], "%d");
	#endif

	// fflush(stdout);
	// sleep(1);

	while(true)
	{
		prompt(NULL, "%lf", A);
		prompt(NULL, "%lf", B);

		write(sock_mul_pc[1], &A, sizeof(A));
		write(sock_mul_pc[1], &A, sizeof(A));
		sem_post(pMul1);
		sem_wait(pMul2);
		read(sock_mul_cp[0], &A, sizeof(A));
		sem_post(pMul2);
		
		write(sock_mul_pc[1], &B, sizeof(B));
		write(sock_mul_pc[1], &B, sizeof(B));
		sem_post(pMul1);
		sem_wait(pMul2);
		read(sock_mul_cp[0], &B, sizeof(B));
		sem_post(pMul2);
		
		write(sock_sum_pc[1], &A, sizeof(A));
		write(sock_sum_pc[1], &B, sizeof(B));
		sem_post(pSum1);
		sem_wait(pSum2);
		read(sock_sum_cp[0], &A, sizeof(A));
		sem_post(pSum2);
		
		write(sock_sqrt_pc[1], &A, sizeof(A));
		sem_post(pSqrt1);
		sem_wait(pSqrt2);
		read(sock_sqrt_cp[0], &A, sizeof(A));
		sem_post(pSqrt2);

		printf("result: %lf\n", A);
	}
	return 0;
}
