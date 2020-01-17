/*SYSC4001A_W2019 L4 */
/* 
 * Muhammad Tarequzzaman 100954008
 * Caleb Gryfe		 101009798
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <signal.h>
#include "part1.h"

#define THRESHOLD 80
void matrixAssign(struct shared_use_st *shared_stuff);
void matrixProduct(struct shared_use_st *shared_stuff);
void printMatricies(struct shared_use_st *shared_stuff);
void childP1(struct shared_use_st *shared_stuff);
void childP2(struct shared_use_st *shared_stuff);
void childP3(struct shared_use_st *shared_stuff);
void childP4(struct shared_use_st *shared_stuff);

int main() {
	int running = 1, runParent = 1, N = 4;
	pid_t pids[N];

	void *shared_memory = (void *) 0;
	struct shared_use_st *shared_stuff;
	int shmid;
	srand((unsigned int) getpid());
	shmid = shmget((key_t) 1234, sizeof(struct shared_use_st),
			0666 | IPC_CREAT);

	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(shmid, (void *) 0, 0);

	if (shared_memory == (void *) -1) {
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	shared_stuff = (struct shared_use_st *) shared_memory;

	shmid = shmget((key_t) 1234, sizeof(struct shared_use_st),
			0666 | IPC_CREAT);

	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(shmid, (void *) 0, 0);
	if (shared_memory == (void *) -1) {
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	/* Use This part to see if multiplication works */
	/* matrixAssign(shared_stuff);
	 printMatricies(shared_stuff);
	 childP1(shared_stuff);
	 childP2(shared_stuff);
	 childP3(shared_stuff);
	 childP4(shared_stuff);
	 printMatricies(shared_stuff);
	 shmdt(shared_memory);
	 shmctl(shmid, IPC_RMID, NULL);
	 /**/

	matrixAssign(shared_stuff);
	int i = 0;
	int count = 0;
	for (i = 0; i < N; ++i) {
		if ((pids[i] = fork()) == 0) {
			if (i == 0) {

				childP1(shared_stuff);
				printf("\n PID of child %d = %d\n", i, pids[i]);

			} else if (i == 1) {
				childP2(shared_stuff);
				printf("\n PID of child %d = %d\n", i, pids[i]);

			} else if (i == 2) {
				childP3(shared_stuff);
				printf("\n PID of child %d = %d\n", i, pids[i]);

			} else if (i == 3) {
				childP4(shared_stuff);
				printf("\n PID of child %d = %d\n", i, pids[i]);

			}
			break;

		}
		if (pids[i] < 0) {//  Forking failed[
			
			perror("fork creating Error");
			abort();
		} else { // parents
			wait(pids);
			printMatricies(shared_stuff);

			shmdt(shared_memory);
			shmctl(shmid, IPC_RMID, NULL);

		}

	}

	exit(1);
	return 0;
}

void matrixAssign(struct shared_use_st *shared_stuff) {
	int k = 4, j = 4, h = 1;
	for (int i = 0; i < 4; i++) {
		shared_stuff->matrixM[0][i] = i + 1;
		shared_stuff->matrixM[1][i] = i + 5;
		shared_stuff->matrixM[2][i] = j;
		shared_stuff->matrixM[3][i] = j + 4;

		shared_stuff->matrixN[0][i] = h;
		shared_stuff->matrixN[1][i] = (i + 1) * 2;

		shared_stuff->matrixN[3][i] = k + 4;
		h = h + 2;
		j--;
		k = k - 2;
	}
	shared_stuff->matrixN[2][0] = 7;
	shared_stuff->matrixN[2][1] = 3;
	shared_stuff->matrixN[2][2] = 5;
	shared_stuff->matrixN[2][3] = 7;

}
void matrixProduct(struct shared_use_st *shared_stuff) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				shared_stuff->matrixQ[i][j] += shared_stuff->matrixM[i][k]
						* shared_stuff->matrixN[k][j];
			}
		}
	}
}
void childP1(struct shared_use_st *shared_stuff) {

	printf("Child Process: working with row %d contains: ", 0);
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[0][0] += shared_stuff->matrixM[0][k]
				* shared_stuff->matrixN[k][0];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[0][1] += shared_stuff->matrixM[0][k]
				* shared_stuff->matrixN[k][1];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[0][2] += shared_stuff->matrixM[0][k]
				* shared_stuff->matrixN[k][2];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[0][3] += shared_stuff->matrixM[0][k]
				* shared_stuff->matrixN[k][3];
	}
	printf("%d %d %d %d\n", shared_stuff->matrixQ[0][0],
			shared_stuff->matrixQ[0][1], shared_stuff->matrixQ[0][2],
			shared_stuff->matrixQ[0][3]);
}
void childP2(struct shared_use_st *shared_stuff) {

	printf("Child Process: working with row %d contains: ", 1);
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[1][0] += shared_stuff->matrixM[1][k]
				* shared_stuff->matrixN[k][0];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[1][1] += shared_stuff->matrixM[1][k]
				* shared_stuff->matrixN[k][1];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[1][2] += shared_stuff->matrixM[1][k]
				* shared_stuff->matrixN[k][2];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[1][3] += shared_stuff->matrixM[1][k]
				* shared_stuff->matrixN[k][3];
	}
	printf("%d %d %d %d\n", shared_stuff->matrixQ[1][0],
			shared_stuff->matrixQ[1][1], shared_stuff->matrixQ[1][2],
			shared_stuff->matrixQ[1][3]);
}

void childP3(struct shared_use_st *shared_stuff) {

	printf("Child Process: working with row %d contains: ", 2);
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[2][0] += shared_stuff->matrixM[2][k]
				* shared_stuff->matrixN[k][0];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[2][1] += shared_stuff->matrixM[2][k]
				* shared_stuff->matrixN[k][1];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[2][2] += shared_stuff->matrixM[2][k]
				* shared_stuff->matrixN[k][2];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[2][3] += shared_stuff->matrixM[2][k]
				* shared_stuff->matrixN[k][3];
	}
	printf("%d %d %d %d\n", shared_stuff->matrixQ[2][0],
			shared_stuff->matrixQ[2][1], shared_stuff->matrixQ[2][2],
			shared_stuff->matrixQ[2][3]);

}
void childP4(struct shared_use_st *shared_stuff) {

	printf("Child Process: working with row %d contains: ", 3);
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[3][0] += shared_stuff->matrixM[3][k]
				* shared_stuff->matrixN[k][0];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[3][1] += shared_stuff->matrixM[3][k]
				* shared_stuff->matrixN[k][1];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[3][2] += shared_stuff->matrixM[3][k]
				* shared_stuff->matrixN[k][2];
	}
	for (int k = 0; k < 4; k++) {
		shared_stuff->matrixQ[3][3] += shared_stuff->matrixM[3][k]
				* shared_stuff->matrixN[k][3];
	}
	printf("%d %d %d %d\n", shared_stuff->matrixQ[3][0],
			shared_stuff->matrixQ[3][1], shared_stuff->matrixQ[3][2],
			shared_stuff->matrixQ[3][3]);
}
void printMatricies(struct shared_use_st *shared_stuff) {
	printf("Matrix M\n");
	for (int k = 0; k < 4; k++) {
		for (int l = 0; l < 4; l++) {
			printf(" %d", shared_stuff->matrixM[k][l]);

		}
		printf("\n");
	}
	printf("\n");
	printf("Matrix N\n");
	for (int k = 0; k < 4; k++) {
		for (int l = 0; l < 4; l++) {
			printf(" %d", shared_stuff->matrixN[k][l]);

		}
		printf("\n");
	}
	printf("\n");
	printf("The Product of MxN=Q is:\n");
	for (int k = 0; k < 4; k++) {
		for (int l = 0; l < 4; l++) {
			printf(" %d", shared_stuff->matrixQ[k][l]);

		}
		printf("\n");
	}
}
