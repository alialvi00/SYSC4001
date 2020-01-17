/* SYSC 4001 Assignment 2
 *
 * Muhammad Tarequzzaman 100954008
 * Caleb Gryfe		101009798
 *
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <pthread.h>
#include <wait.h>

#include "STATS.h"

# define SEM_ID 250

void createDelay() {
	static int init = 0;
	int randVal;
	struct timespec delay; // Busy waiting structure

	if (!init) {
		srand(time(NULL));
		init = 1;
	}

	randVal = rand() % 10; // Create a random number for delay
	delay.tv_sec = 0; // Assign Delay Seconds
	delay.tv_nsec = 10 * randVal; // Assign Delay Nano Seconds
	nanosleep(&delay, NULL); // Sleep for specified time
}

void sem_lock(int sem_set_id) {
	struct sembuf sem_op; //Semaphore operations structure

	//Condition to hold semaphone until value contained is positive
	sem_op.sem_num = 0;
	sem_op.sem_op = -1;
	sem_op.sem_flg = 0;
	semop(sem_set_id, &sem_op, 1); //Linux Definition: int semop(int semid, struct sembuf *sops, size_t nsops);
}

void sem_unlock(int sem_set_id) {
	struct sembuf sem_op;

	sem_op.sem_num = 0;
	sem_op.sem_op = 1; //Positive value to indicate unlocked semaphore
	sem_op.sem_flg = 0;
	semop(sem_set_id, &sem_op, 1);
}

int main(int argc, char* argv[]) {
	int debMode = 0; // Value to track Debug mode

	int rc, shmArraySize;
	int sem_set_id;
	union semun { // This is for the fourth argument in: int semctl(int semid, int semnum, int cmd, union semun arg);
		int val;
		struct semid_ds *buf;
		ushort * array;
	} sem_val;

	int shmid;

	sem_set_id = semget(SEM_ID, 1, IPC_CREAT | 0600); // Semaphore id assignment
	if (sem_set_id == -1) {
		perror("main: sem_set_id.semget");
		exit(1);
	}

	sem_val.val = 1; // Assign value of 1 to semaphore
	rc = semctl(sem_set_id, 0, SETVAL, sem_val); // Assign rc to semctl in order to perform control operations on the system semaphore structure
	if (rc == -1) {
		perror("main: semctl");
		exit(1);
	}

	/*	Shared Memory set up , *arr */
	int *shm_addr, *array;
	size_t shmSize = 2048;
	key_t key = 100; // this controls the size of actual shmget capacity.
	shmid = shmget(key, shmSize * sizeof(int), 0666 | IPC_CREAT);

	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shm_addr = shmat(shmid, 0, 0); // shared memory adddress

	if (shm_addr == (int *) -1) {
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	array = ((void*) shm_addr + sizeof(int));

	char str1; // str1 used to store user input
	printf(
			"Would you like to run in Debug Mode? Enter y for yes, otherwise enter any Key\n");
	scanf("%c", &str1);
	if (str1 == 'y') {
		debMode = 1;
		printf("Debug is on\n");
	} else {
		debMode = 0;
		printf("Debug is off\n");
	}

	int in1, in2, in3, in4, in5; // input variables for used to enter distinct integers

	printf(
			"Please enter 5 distinct integers (Format: Press Key, Press Enter): \n");
	scanf("%d%d%d%d%d", &in1, &in2, &in3, &in4, &in5);
	printf("You have Entered: %d, %d, %d, %d, %d\n", in1, in2, in3, in4, in5);

	int input[5] = { in1, in2, in3, in4, in5 }; // Assign array with the scanned integer values

	int inputArraySize;
	inputArraySize = sizeof(input) / sizeof(input[0]); // Obtain size of array
	initialize(input, array, inputArraySize); // Call to initialize the array
	int final[inputArraySize];

	int child = 0, m = 0;
	pid_t pid;

	/*Child Process Created for inputArraySize -1,
	 * i.e. if Size of array is 5, 4 process creates,
	 * one for each two elements plus extra for sort success */
	while (m < inputArraySize) {
		if ((pid = fork()) == 0) {
			break;
		};
		m++;
	}

	switch (pid) {
	case -1:
		perror("fork: ");
		exit(1);
		break;
	case 0: /*Child Process*/

		do {
			child++;

			p1Sort(sem_set_id, array, debMode);
			p2Sort(sem_set_id, array, debMode);
			p3Sort(sem_set_id, array, debMode);
			p4Sort(sem_set_id, array, debMode);

			child--;
		} while (!(array[0] >= array[1] && array[1] >= array[2]
				&& array[2] >= array[3] && array[3] >= array[4]));

		break;
	default: /*Parents Process*/
	{
		wait(&pid);
		if (child == 0) {
			sortedArrayOutput(array, final, inputArraySize);
		}
		break;
	}

	}
	shmdt(array);
	if (shmdt(shm_addr) == -1) {
		perror("main: shmdt: ");
	}
	/* de-allocate the shared memory segment. */
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}

void initialize(int *input, int *output, int size) {
	int i;
	for (i = 0; i < size; i++) { //Assign arrays
		output[i] = input[i];
	}
}

void sortedArrayOutput(int *input, int *output, int outputArraySize) {
	int i;
	for (i = 0; i < outputArraySize; i++) { //Assign arrays
		output[i] = input[i];
	}
	int c = 6;
	while (c != 0) { // Process checking
		c--;
		createDelay();
	}
	printList(output, outputArraySize);
}

void p1Sort(int sem_set_id, int *a, int dbug) {
	sem_lock(sem_set_id); // Lock while in this funciton as to not allow multiple processes accessing the same data
	int temp1;
	if (a[0] < a[1]) { // implement of index switching in array
		temp1 = a[0];
		a[0] = a[1];
		a[1] = temp1;
		if (dbug) { // If user entered debug mode then print information
			printf("Process P1: Performed Swapping\n");
		}
	} else {
		if (dbug) {
			printf("Process P1: No Swapping\n");
		}
	}
	createDelay(); // Delay to ensure mutual exclusion
	sem_unlock(sem_set_id); // Release lock to allow next process to run
}
void p2Sort(int sem_set_id, int *a, int dbug) {
	sem_lock(sem_set_id);
	int temp2;
	if (a[1] < a[2]) {
		temp2 = a[1];
		a[1] = a[2];
		a[2] = temp2;
		if (dbug) {
			printf("Process P2: Performed Swapping\n");
		}
	} else {
		if (dbug) {
			printf("Process P2: No Swapping\n");
		}
	}
	createDelay();
	sem_unlock(sem_set_id);
}
void p3Sort(int sem_set_id, int *a, int dbug) {
	sem_lock(sem_set_id);
	int temp3;
	if (a[2] < a[3]) {
		temp3 = a[2];
		a[2] = a[3];
		a[3] = temp3;
		if (dbug) {
			printf("Process P3: Performed Swapping\n");
		}
	} else {
		if (dbug) {
			printf("Process P3: No Swapping\n");
		}
	}
	createDelay();
	sem_unlock(sem_set_id);
}
void p4Sort(int sem_set_id, int *a, int dbug) {
	sem_lock(sem_set_id);
	int temp4;
	if (a[3] < a[4]) {
		temp4 = a[3];
		a[3] = a[4];
		a[4] = temp4;
		if (dbug) {
			printf("Process P4: Performed Swapping\n");
		}
	} else {
		if (dbug) {
			printf("Process P4: No Swapping\n");
		}
	}
	createDelay();
	sem_unlock(sem_set_id);
}

void printList(int *a, int size) {
	printf("\nThe Array is now Sorted in Decreasing Value and Contains: \n");
	for (int i = 0; i < size; i++) { // Iterate length of array and print all indecies
		printf(" %d ", a[i]);
	}
	printf("\n\n");
	printf("The Minimum Value is: %d\n", a[size - 1]); // Min value is stored at end since we sorted in Decreasing order
	printf("The Maximum Value is: %d\n", a[0]); // Max value is stored at beginning since we sorted in Decreasing order
	printf("The Median Value is: %d\n", a[(size - 1) / 2]); // Median is middle value of the array

}
