#pragma once

#include <stdlib.h>
#include <stdio.h>

namespace MDS
{

#pragma pack(push, 1)

class Data
{
protected:
    union
    {
        int integer;
        void *pointer;
    } value;
public:
    Data(int i = 0);
    Data(void *p);

    int integer();
    void * pointer();
};

class MessageLoop;

class Object
{
private:
    MessageLoop *loop;

public:
    virtual bool processMessage(int message, Data extra_data) = 0;

    MessageLoop *getLoop();
    void setLoop(MessageLoop *loop);

    Object();
    virtual ~Object();
};

class QueueItem;

class MessageLoop
{
private:
    // internals
    static MessageLoop *running; // pointer to currently running MessageLoop (NULL if none)
    QueueItem   *queue;
    void        afterRun();

protected:
    // FOR ALL: if true leave run()
    virtual bool processMessage(int message, Data extra_data) = 0;
    virtual bool processKey(int ch) = 0;
    virtual bool processIdle() = 0;

    // you can place some code in this function
    // which will be executed before every run
    virtual void beforeRun();

    // functions for working with UI
    virtual int getKey() = 0;
    virtual int keyPressed() = 0;

public:
    MessageLoop();
    virtual ~MessageLoop();

    bool    sendMessage (Object *obj, int message, Data extra_data);
    void    postMessage (Object *obj, int message, Data extra_data, unsigned delay = 0);

    void    run();
};

#pragma pack(pop)
}
