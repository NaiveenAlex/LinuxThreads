#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


static void*
thread_fn_callback(void *arg)
{
    char *input = (char *) arg;
    int a = 0;
    while(1)
    {
        printf("Input String %s.\n", input);
        sleep(1);
        a++;
        if(a == 10)
        {
            pthread_exit(0);
        }    
    }

}


void thread1_create()
{
    pthread_t pthread1;  //Thread handle to identify the thread 
    static char *thread_input1 = "This is thread no 1.";

    int rc = pthread_create(&pthread1,                  //fork point
                            NULL,
                            thread_fn_callback,
                            (void*)thread_input1);      //argument must not be a local 
                                                        //variable as fork point function returns.

    if(rc != 0)
    {
        printf("Error occured, thread could not be created, errno=%d\n", rc);
        exit(0);
    }
}

int main(int argc, char** argv)
{

    thread1_create();
    printf("main function paused.\n");
    //pause();
    pthread_exit(0);
    return 0;
}
