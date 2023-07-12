/*
    gcc JoinDetach.cpp -o JoinDetach.o -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

pthread_t pthread1;
pthread_t pthread2;


void*
thread_fn_callback(void *arg)
{
    int th_id = *(int*) arg;
    free(arg);

    int rc = 0;

    while(rc != th_id)
    {
        printf("Thread %d doing work.\n", th_id);
        sleep(1);
        rc++;
    }

    int *presult = (int*)calloc (1, sizeof(int));
    *presult = th_id * th_id;

    return (void*)presult;
}


void thread_create(pthread_t *pthread_handle, int th_id)
{
    pthread_attr_t attr;
    
    pthread_attr_setdetachstate(&attr, 
        PTHREAD_CREATE_JOINABLE);

    int* _pth_id = (int*)calloc(1, sizeof(int));
    *_pth_id = th_id;
    
    pthread_create(pthread_handle,
        &attr,
        thread_fn_callback,
        (void*)_pth_id);
    

}

int main(int argc, char** argv)
{

    thread_create(&pthread1, 2);
    thread_create(&pthread2, 10);

    void* pthread_result1, *pthread_result2;

    printf("main blocked on pthread join for th_id 1.\n");
    pthread_join(pthread1, &pthread_result1);

    if(pthread_result1)
    {
        printf("Return result from thread2 = %d.\n", *(int*)pthread_result1);
        free(pthread_result1);
    }

    printf("main blocked on pthread join for th_id 2.\n");
    pthread_join(pthread2, &pthread_result2);

    if(pthread_result2)
    {
        printf("Return result from thread2 = %d.\n", *(int*)pthread_result2);
        free(pthread_result2);
    }

    return 0;
}
