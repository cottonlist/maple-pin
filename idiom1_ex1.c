#include <stdio.h>
#include <pthread.h>

static int a = 0;

void *func1(void *arg) {
	// printf("Accessing a in th1, %d\n", a);
	a = 444;
	// printf("Thread1");
	// printf("Thread1");
	// printf("Thread1");
	// printf("Thread1");
	// printf("Thread1");
	// printf("Thread1");
	printf("Value of a: %d\n", a);

	return NULL;
}

void *func2(void *arg) {
	// printf("Accessing a in th2, %d\n", a);
	a = 666;
	// printf("Thread2");
	// printf("Thread2");
	// printf("Thread2");
	// printf("Thread2");
	// printf("Thread2");
	// printf("Thread2");
	// printf("Thread2");
	// printf("Thread2");
	// printf("Thread2");

	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t t1;
	pthread_t t2;

	pthread_create(&t1, NULL, func1, NULL);
	pthread_create(&t2, NULL, func2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Final value of a is %d.\n", a);

	return 0;
}