/*
 * Threadpool.h
 *
 * Thread pool declarations.
 *
 *      Author: Hirad Asadi
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <exception>

#include "WorkThread.h"

class ThreadPool
{
    class threadexception: public std::exception
    {
        virtual const char* what() const throw ()
        {
            return "Exception in ThreadPool(): # of threads < 1 !";
        }
    } threadex;

    static unsigned int T_ID;

private:

    std::atomic_bool started;
    size_t nrOfThreads;
    std::queue<std::function<void()> > workQueue;
    std::vector<WorkThread*> liveThreads;
    std::condition_variable cVar;
    std::mutex threadMutex;

    void clearThreads();

public:

    ThreadPool(size_t);
    virtual ~ThreadPool();

    void start();
    void stop();
    void clearWork();
    size_t queueSize();

    template<class T>
    void addWork(const T t)
    {
        std::unique_lock<std::mutex> tlock(threadMutex);
        workQueue.push(t);
        tlock.unlock();
        cVar.notify_one();
    }

};

#endif /* THREADPOOL_H_ */
