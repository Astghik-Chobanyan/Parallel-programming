#include <iostream>
#include <pthread.h>
#include <string.h>

void *write_in_buff(void *);
void  *read_from_buff(void *);

const int max_size = 10;
char buff[max_size];
pthread_t thr1, thr2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t variable = PTHREAD_COND_INITIALIZER;

int main()
{
    pthread_create(&thr1, NULL, write_in_buff, NULL);
    pthread_create(&thr2, NULL, read_from_buff, NULL);

    pthread_join(thr2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void *write_in_buff(void *)
{
    char str[max_size];
    while (true)
    {
        std::cin >> str;
        pthread_mutex_lock(&mutex);
        strncpy(buff, str, max_size);
        pthread_cond_signal(&variable);
        pthread_mutex_unlock(&mutex);
    }
}

void  *read_from_buff(void *)
{
    while (true)
    {
        pthread_cond_wait(&variable, &mutex);
        std::cout << buff << std::endl;
        pthread_mutex_unlock(&mutex);
    }
}