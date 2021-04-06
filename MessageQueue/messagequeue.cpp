#include "messagequeue.h"
#include <algorithm>

pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t thr, thr1;

void *message_broadcaster(void *param)
{
    bool stop = false;
    while(!stop)
    {
        std::string message;
        message_queue *m_queue = (message_queue *)param;
        pthread_mutex_lock(&mutex);
        message = m_queue->_queue.front();
        m_queue->_queue.pop();
        if(m_queue->_queue.empty())
        {
            stop = true;
        }
        pthread_mutex_unlock(&mutex);
        for(auto i = m_queue->_functions.begin(); i != m_queue->_functions.end(); i++)
        {
            (*i)(message);
        }
    }
    return 0;
}

void *handler_function(void *param)
{
    std::string message;
    message_queue *m_queue = (message_queue *)param;
    while(1)
    {
        std::cout << "Wainting for message " << std::endl;
        pthread_cond_wait(&cond_var, &mutex);
        pthread_create(&thr1, NULL, message_broadcaster, m_queue);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

message_queue::message_queue()
{
    pthread_create(&thr, NULL, handler_function, this);
    sleep(1);
}

message_queue::~message_queue()
{
}

void message_queue::register_handler(message_handler handler)
{
    _functions.push_back(handler);
}

void message_queue::post_message(std::string message)
{
    pthread_mutex_lock(&mutex);
    std::cout << "Message is " << message << std::endl;
    _queue.push(message);
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&mutex);
}