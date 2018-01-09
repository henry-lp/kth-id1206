#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

/* The list will contain elements from 0 to 99 */

#define MAX 100

typedef struct cell {
	int val;
	struct cell *next;
	int mutex;
} cell;

cell sentinel = {MAX, NULL, 0};
cell dummy = {-1, &sentinel, 0};

cell *global = &dummy;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int try(volatile int *mutex) {
	return __sync_val_compare_and_swap(mutex, 0, 1);
}

void lock(volatile int *mutex) {
	whil(try(mutex) != 0); // spin
}

void unlock(volatile int *mutex) {
	*mutex = 0;
}

// takes a pointer to the list and a value that it will
// either add or remove. We use two pointers to run through
// the list, this and prev. If we find the element we can
// remove the cell since we keep a pointer to the previous
// cell. We take the mute lock before we start to prevent
// conflicts with other operations.
void toggle(cell *lst, int r) {
	cell *prev = lst;
	lock(mutex);
	cell *this = prev -> next;
	lock(mutex);

	cell *removed = NULL;

	while(this -> val < r) {
		pthread_mutex_unlock(&prev -> mutex);
		prev = this;
		this = this -> next;
		pthread_mutex_lock(&this -> mutex);
	}

	if(this -> val == r) {
		prev -> next = this -> next;
		removed = this;
	} else {
		cell *new = malloc(sizeof(cell));
		new -> val = r;
		new -> next = this;
		pthread_mutex_init(&new -> mutex, NULL);
		prev -> next = new;
		new = NULL;
	}

	unlock(&mutex);
	unlock(&mutex);
	if(removed != NULL) {
		free(removed);
	}

	return;
}


typedef struct {
	int inc;
	int id;
	cell *list;
} args;


// benchmark thread loops and toggles random numbers in the list.
void *bench(void *arg) {
	int inc = ((args*) arg) -> inc;
	int id = ((args*) arg) -> id;
	cell *lstp = ((args*) arg) -> list;

	for(int i = 0; i < inc; i++) {
		int r = rand() % MAX;
		toggle(lstp, r);
	}
}

// starts a number of threads and let them run a
// a benchmark procedure each.
int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("usage: list <total><threads>\n");
		exit(0);
	}
	int n = atoi(argv[2]);
	int inc = (atoi(argv[1]) / n);

	printf("%d threads doing %d operations each\n", n, inc);

	struct timepsec t_start, t_stop;

	clock_gettime(CLOCK_MONOTONIC_COARSE, &t_start);

	pthread_mutex_init(&mutex, NULL);

	args *thra = malloc(n * sizeof(args));
	for(int i = 0; i < n; i++) {
		thra[i].inc = inc;
		thra[i].id = i;
		thra[i].list = global;
	}

	pthread_t *thrt = malloc(n * sizeof(pthread_t));
	for(int i = 0; i < n; i++) {
		pthread_create(&thrt[i], NULL, bench, &thra[i]);
	}

	for(int i = 0; i < n; i++) {
		pthread_join(thrt[i], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC_COARSE, &t_stop);

	long wall_sec = t_stop.tv_sec - t_start.tv_sec;
	long wall_nsec = t_stop.tv_nsec - t_start.tv_nsec;
	long wall_msec = (wall_sec *1000) + (wall_nsec / 1000000);

	printf("done in %ld ms\n", wall_msec);

	printf("done \n");
	return 0;
}
