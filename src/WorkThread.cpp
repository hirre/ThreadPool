/*
 * WorkThread.cpp
 *
 * Implementation of WorkThread.
 *
 *      Author: Hirad Asadi
 */

#include "WorkThread.h"

#include <iostream>

using namespace std;

WorkThread::WorkThread(unsigned int _id, queue<function<void()> >& workQueue,
        mutex& threadMutex, condition_variable& cVar) :
        _ID(_id)
{
    this->cVar = &cVar;
    this->threadMutex = &threadMutex;
    this->workQueue = &workQueue;
    this->running = false;
    this->t = NULL;
}

WorkThread::~WorkThread()
{
    running = false;
    if (t != NULL)
        delete t;
}

thread* WorkThread::getThread()
{
    return t;
}

bool WorkThread::isRunning()
{
    return running;
}

void WorkThread::stop()
{
    if (!running)
        return;
    running = false;
    cVar->notify_all();
    t->join();
}

void WorkThread::start()
{
    if (running)
        return;

    running = true;

    // Create new thread
    t = new thread(&WorkThread::wThread, this);
}

void WorkThread::wThread()
{
    // Loop while running
    while (running)
    {
        unique_lock<mutex> tlock(*threadMutex);
        //cout << "Thread(" << _ID << ") working..." << endl;

        // Wait while queue is empty
        while (running && workQueue->empty())
        {
            cVar->wait(tlock, [&]()
            {   // Return if notification was missed
                return !( workQueue->empty() && running );
            });
        }

        if (running)
        {
            // Remove first item from queue
            function<void()> run = workQueue->front();
            workQueue->pop();

            // Run work within this thread
            run();
        }

        tlock.unlock();

        if (!running)
            return;
    }

    running = false;
}
