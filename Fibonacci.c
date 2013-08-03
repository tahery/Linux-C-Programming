#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_t ntid;
int fibonacci[200];

void * thread_fun(void *many)
{
	// initialize the fibnacci array
	fibonacci[0] = 1;
	fibonacci[1] = 1;
	int n = (int)many;
	int i;
	// calculate the fibnacci array
	for (i = 2; i < n; ++i)
	{
		fibonacci[i] = fibonacci[i-1] + fibonacci[i-2];
	}
	return NULL;
}

int main()
{
	int newthread;
	int many;
	int i;
	printf("how many items?");
	// get the number of items
	scanf("%d", &many);
	// create new thread
	newthread = pthread_create(&ntid, NULL, thread_fun, (void*)many);
	// wait for the tid
	pthread_join(ntid, NULL);
	// print it out
	for (i = 0; i < many; ++i)
	{
		printf("%d\n", fibonacci[i]);
	}
}