#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * tmpfunction( void * something ) {}

int main()
{
	pthread_t thread1;
	int ret1;
	unsigned long long tmp = 0;

	ret1 = pthread_create( &thread1, NULL, tmpfunction, ( void * )tmp );
	pthread_join( thread1, NULL);

	return 0;
}