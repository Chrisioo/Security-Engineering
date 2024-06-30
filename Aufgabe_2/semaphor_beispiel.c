#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>

#define N_DATA 2000000
#define N_SHARED 20000

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;

void ex_semop(int sem_id, int sem_num, int sem_op) {
	struct sembuf sop;
	sop.sem_num = sem_num;
	sop.sem_op = sem_op;
	sop.sem_flg = 0;
	if (semop(sem_id, &sop, 1) == -1) {
		perror("semop error");
		exit(EXIT_FAILURE);
	}
}

int main() {

	//Create array
	int numbers[N_DATA];
	for (int i=0; i<N_DATA; i++) {
		numbers[i] = 0;
	}

	//Create shm and sem-set
	int shm_id = shmget(IPC_PRIVATE, (sizeof(int)*N_SHARED), IPC_CREAT | 0666);
	if (shm_id == -1) {
		perror("shmget error");
		exit(EXIT_FAILURE);
	}
	int sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
	if (sem_id == -1) {
		perror("semget error");
		exit(EXIT_FAILURE);
	}

	union semun sem_union;
	sem_union.val = 0;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1) { //Init S0=0
		perror("error initializing S0");
		exit(EXIT_FAILURE);
	}

	sem_union.val = 1;
	if (semctl(sem_id, 1, SETVAL, sem_union) == -1) { //Init S1=1
		perror("error initializing S1");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork(); //Fork

	if (pid<0) {
		perror("Fork fail\n");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		int* shared_memory = shmat(shm_id, NULL, 0);
		if (shared_memory == (int*)(-1)) {
			perror("shmat error in child process");
			exit(EXIT_FAILURE);
		}

		printf("Child process array after fork:\n");

		for (int i=0; i<N_DATA; i++) {
			printf("%d, ", numbers[i]);
		}
		printf("\n");

		for (int i=0; i < (N_DATA / N_SHARED); i++) {
			ex_semop(sem_id, 0, -1); //Change S0 from 1 to 0

			for(int j=0; j<N_SHARED; j++) {
				numbers[i*N_SHARED+j] = shared_memory[j];
			}

			ex_semop(sem_id, 1, 1); //Change S1 from 0 to 1
		}

		printf("Child process array after read:\n");

		for (int i=0; i<N_DATA; i++) {
			printf("%d, ", numbers[i]);
		}
		printf("\n");


		if (shmdt(shared_memory) == -1) { //Detach the shared memory from the child process
			perror("shmdt error in child process");
			exit(EXIT_FAILURE);
		}

	} else {
		int* shared_memory = shmat(shm_id, NULL, 0);
		if (shared_memory == (int*)(-1)) {
			perror("shmat error in parent process");
			exit(EXIT_FAILURE);
		}

		printf("Parent process array after fork:\n");

		for (int i=0; i<N_DATA; i++) {
			printf("%d, ", numbers[i]);
		}
		printf("\n");

		srand48(time(NULL));
		for (int i=0; i<N_DATA; i++) {
			numbers[i] = (int)lrand48();
		}

		printf("Parent process generated numbers:\n");

		for (int i=0; i<N_DATA; i++) {
			printf("%d, ", numbers[i]);
		}
		printf("\n\n");

		for (int i=0; i < (N_DATA / N_SHARED); i++) {
			ex_semop(sem_id, 1, -1); //Change S1 from 1 to 0
	
			for (int j=0; j < N_SHARED; j++) {
				shared_memory[j] = numbers[i*N_SHARED+j];
			}

			ex_semop(sem_id, 0, 1); //Change S0 from 0 to 1
		}

		wait(NULL); //Wait until the child process finishes

		if (shmdt(shared_memory) == -1) { //Detach the shared memory from the parent process
			perror("smdt error in parent process");
			exit(EXIT_FAILURE);
		}

		if (shmctl(shm_id, IPC_RMID, NULL) == -1) { //Delete the shared memory segment
			perror("Error deleting shm segment");
			exit(EXIT_FAILURE);
		}

		if (semctl(sem_id, 0, IPC_RMID) == -1) { //Delete the semaphore set
			perror("Error deleting sem set");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
