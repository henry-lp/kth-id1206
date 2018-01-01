// Writes out a sequence of random numbers.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* 20% of the pages will have 80% of the references */
#define HIGH 20
#define FREQ 80
#define PAGES 100
#define SAMPLES 20

typedef struct pte {
	int id;
	int present;
	int referenced;
	struct pte *next;
} pte;

void clear_page_table(pte *page_table, int pages) {
	for(int i = 0; i < pages; i++) {
		page_table[i].present = 0;
	}
}

void init(int *sequence, int refs, int pages) {

	int high = (int) (pages *((float) HIGH / 100));

	for(int i = 0; i < refs; i++) {
		if(rand() % 100 < FREQ) {
			/* the frequency case */
			sequence[i] = rand() % high;
		} else {
			/* the less frequently case */
			sequence[i] = high + rand() % (pages - high);
		}
	}
}

int simulate(int *seq, pte *table, int refs, int frms, int pgs) {

	int hits = 0;
	int allocated = 0;

	pte *last = NULL;

	for(int i = 0; i < refs; i++) {
		int next = seq[i];
		pte *entry = &table[next];

		if(entry -> present == 1) {
			entry -> referenced = 1;
			hits++;
		} else {
			if(allocated < frms) {
				allocated++;
				entry -> present = 1;

				/* place the entry last in the list */
			} else {
				pte *cand = last -> next;

				while(cand -> referenced != 0) {
					cand -> referenced = 0;
					last = cand;
					cand = cand -> next;
				}
				cand -> present = 0;
				cand -> referenced = 0;

				entry -> present = 1;
				entry -> referenced = 0;

				/* place the entry last in the list */
			}
		}
	}
	return hits;
}

int main(int argc, char *argv[]) {

	pte *table = (pte *) malloc(PAGES *sizeof(pte));

	/* could be command line arguments */
	int refs = 100000;
	int pages = 100;

	int *sequence = (int*) malloc (refs *sizeof(int));

	init(sequence, refs, pages);


	/* compute a increment, the number of pages divided by the number
	of data pints, and then run the simulation for a growing number
	of frames. */

	printf("# This is a benchmark of random replacement\n");
	printf("# %d page references\n", refs);
	printf("# %d pages \n", pages);
	printf("#\n# frames\tratio\n");

	/* frames is the size of the memory in frames */
	int frames;

	int incr = pages / SAMPLES;

	for(frames = incr; frames <= pages; frames += incr) {
		/* clear page tables entries */
		clear_page_table(table, pages);

		int hits = simulate(sequence, table, refs, frames, pages);

		float ratio = (float) hits / refs;

		printf("%d\t%.2f\n", frames, ratio);
	}

	return 0;
}
