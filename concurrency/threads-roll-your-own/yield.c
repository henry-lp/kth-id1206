#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>

#define MAX 10

// either 1 or 2 depending on which context is executing.
static int running;
static int done1;
static int done2;

static ucontext_t cntx_one;
static ucontext_t cntx_two;
static ucontext_t cntx_main;
// swith to this context when a context terminates
static ucontext_t cntx_done;

// allows us to switch between two contexts
void yield() {
	printf(" -  yield -\n");
	if(running == 1) {
		running = 2;
		swapcontext(&cntx_one, &cntx_two);
	} else {
	running = 1;
	swapcontext(&cntx_two, &cntx_one);
	}
}

// makes the stack grow; print out when a call to push
// is made and also on the way up.
void push(int p, int i) {
	if(i < MAX) {
		printf("%d%*s push\n", p, i," ");
		push(p, i+1);
		printf("%d%*s pop\n", p, i, " ");
	} else {
		printf("%d%*s top\n", p, i, " ");
		// swap between the contexts
		yield();
	}
}


// is scheduled when one of the contexts terminate and we
// should then switch to the remaining. We keep track of
// which context is still alive so we do some bookkeeping
// using the flags done1, done2 and done.
void done() {

	int done = 0;

	while(!done) {
		if(running == 1) {
			printf(" - process one terminating -\n");
			done1 = 1;
			if(!done2) {
				running = 2;
				swapcontext(&cntx_done, &cntx_two);
			} else {
				done = 1;
			}
		} else {
			printf(" - process two terminating -\n");
			done2 = 1;
			if(!done) {
				running = 1;
				swapcontext(&cntx_done, &cntx_one);
			} else {
				done = 1;
			}
		}
	}

	printf(" - done terminating -\n");
}


int main() {

	// two *Kbyte stacks
	char stack1[8*1024];
	char stack2[8*1024];
	char stack_done[8*1024];

	/* The first context */

	getcontext(&cntx_one);
	// context gets stack of its own
	cntx_one.uc_stack.ss_sp = stack1;
	cntx_one.uc_stack.ss_size = sizeof stack1;
	// sets the context in such a way that when the
	// context terminates we will automatically switch
	// to the context cntx_done.
	cntx_one.uc_link = &cntx_done;
	// provide reference to context and the procedure to
	// call when they start execution.
	makecontext(&cntx_one, (void (*) (void)) push, 2, 2, 2);

	getcontext(&cntx_two);
	// context gets stack of its own
	cntx_two.uc_stack.ss_sp = stack2;
	cntx_two.uc_stack.ss_size = sizeof stack2;
	// sets the context in such a way that when the
	// context terminates we will automatically switch
	// to the context cntx_done.
	cntx_two.uc_link = &cntx_done;
	// provide reference to context and the procedure to
	// call when they start execution
	makecontext(&cntx_two, (void (*) (void)) push, 2, 2, 1);

	// initialise context and give cntx_main to be context
	// to switch to when terminating
	getcontext(&cntx_done);
	cntx_done.uc_link = &cntx_main;
	cntx_done.uc_stack.ss_sp = stack_done;
	cntx_done.uc_stack.ss_size = sizeof stack_done;
	makecontext(&cntx_done, (void (*) (void)) done, 0);

	running = 1;

	printf(" - let's go! \n");
	swapcontext(&cntx_main, &cntx_one);
	printf(" - that's all folks -\n");

	return 0;
}
