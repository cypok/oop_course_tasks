#include "internals.h"

namespace MDS
{

MessageLoop * MessageLoop::running = NULL;

Data::Data(int i)   { value.integer = i; }
Data::Data(void *p) { value.pointer = p; }

int     Data::integer() { return value.integer; }
void *  Data::pointer() { return value.pointer; }

QueueItem::QueueItem(Object *receiver, int message, Data extra_data, clock_t time, QueueItem *next)
{
   this->receiver = receiver;
   this->message = message;
   this->extra_data = extra_data;
   this->time = time;
   this->next = next;
}

Object::Object() : loop(NULL) {}

Object::~Object() {}

MessageLoop *Object::getLoop() { return loop; }
void Object::setLoop(MessageLoop *loop) { this->loop = loop; }

MessageLoop::MessageLoop()
{
    queue = NULL;
}

MessageLoop::~MessageLoop()
{
}

Data MessageLoop::sendMessage(Object *obj, int message, Data extra_data)
{
    if (obj == NULL)
        throw std::invalid_argument( "message receiver is NULL" );
    else if (this != running)
        throw std::runtime_error( "messageLoop is not running" );
    else if (this != obj->getLoop())
        throw std::invalid_argument( "message receiver does not belong to current messageLoop" );

    return obj->processMessage(message, extra_data);
}

void MessageLoop::postMessage(Object *obj, int message, Data extra_data, unsigned delay)
{
    if (this == NULL || this != running)
        throw std::runtime_error( "messageLoop is not running" );
    else if (obj != NULL && this != obj->getLoop())
        throw std::invalid_argument( "message receiver does not belong to current messageLoop" );

    QueueItem *item = new QueueItem(obj, message, extra_data, clock() + clock_t(delay*CLOCKS_PER_SEC/1000));
    if (queue == NULL)
    {
        queue = item;
        item->next = NULL;
    }
    else
    {
        QueueItem *cur = queue;
        if (queue->time > item->time)
        {
            item->next = queue;
            queue = item;
        }
        else
        {
            while (cur->next != NULL && cur->next->time <= item->time)
                cur = cur->next;
            item->next = cur->next;
            cur->next = item;
        }
    }
}

void MessageLoop::afterRun()
{
    running = NULL;

    QueueItem *cur = NULL;
    QueueItem *next = queue;
    while (next != NULL)
    {
        cur = next;
        next = next->next;
        delete cur;
    }
}

void MessageLoop::beforeRun()
{
}

void MessageLoop::run()
{
    if (running)
        throw std::runtime_error( "some MessageLoop is already running" );

    running = this;
    beforeRun();
    try
    {
                for (;;)
                {
                    MsgInfo msg;
                    bool result = false;

                    if (keyPressed())
                        result = processKey(getKey());

                    else if (queue == NULL || queue->time > clock())
                        result = processIdle();

                    else
                    {
                        msg = MsgInfo(*queue);

                        QueueItem * qelem = queue;
                        queue = queue->next;
                        delete qelem;

                        if (msg.receiver != NULL)
                            sendMessage(msg.receiver, msg.message, msg.extra_data);
                        else
                            result = processMessage(msg.message, msg.extra_data);
                    }

                    if ( result )
                    {
                        afterRun();
                        return;
                    }
                }
    }
    catch (...)
    {
        afterRun();
        throw;
    }
    afterRun();
}
}
