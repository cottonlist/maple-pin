#include <stdio.h>
#include <pthread.h>

static int a = 0;
static int b = 0;
static int c = 0;

void *func1(void *arg) {
	a = 1;
	b = 1;
	if (a == 2)
	{
		printf("idiom 2 bug manifest.\n");
	}
	return NULL;
}

void *func2(void *arg) {
	c = 2;
	a++;
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
	
	return 0;
}