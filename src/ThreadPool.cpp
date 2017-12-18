/*
 * ThreadPool.cpp
 *
 * Implementation of ThreadPool.
 *
 *      Author: Hirad Asadi
 */

#include "ThreadPool.h"

#include <iostream>

using namespace std;

// Unique IDs for threads
unsigned int ThreadPool::T_ID = 0;

ThreadPool::ThreadPool(size_t nrOfThreads) :
        nrOfThreads(nrOfThreads)
{
    // At least one thread is needed for construction
    if (nrOfThreads < 1)
        throw threadex;

    started = false;
}

ThreadPool::~ThreadPool()
{
    stop();
    clearThreads();
    clearWork();
}

void ThreadPool::clearThreads()
{
    // Deallocate memory for threads
    for (size_t i = 0; i < liveThreads.size(); ++i)
    {
        if (liveThreads[i] != NULL)
            delete liveThreads[i];
    }

    liveThreads.clear();
}

void ThreadPool::clearWork()
{
    std::unique_lock<std::mutex> tlock(threadMutex);
    // Clear queue
    queue<function<void()> > empty;
    swap(empty, workQueue);
    tlock.unlock();
}

size_t ThreadPool::queueSize()
{
    std::unique_lock<std::mutex> tlock(threadMutex);
    size_t s = workQueue.size();
    tlock.unlock();
    return s;
}

void ThreadPool::start()
{
    if (started)
        return;

    clearThreads();

    for (size_t i = 0; i < nrOfThreads; i++)
    {
        // Spawn new worker thread
        liveThreads.push_back(
                new WorkThread(ThreadPool::T_ID++, workQueue, threadMutex,
                        cVar));
        liveThreads[i]->start();
    }

    started = true;
}

void ThreadPool::stop()
{
    if (!started)
        return;

    // Stop workers
    for (size_t i = 0; i < liveThreads.size(); ++i)
    {
        if (liveThreads[i] != NULL)
        {
            liveThreads[i]->stop();
        }
    }

    started = false;
}
