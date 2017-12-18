/*
 * WorkThread.h
 *
 * Worker thread that is used by ThreadPool to create work.
 *
 *      Author: Hirad Asadi
 */

#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class WorkThread
{
    friend class ThreadPool;

private:
    const unsigned int _ID;
    std::atomic_bool running;
    std::thread* t;
    std::queue<std::function<void()> >* workQueue;
    std::mutex* threadMutex;
    std::condition_variable* cVar;

    WorkThread(unsigned int, std::queue<std::function<void()> >&, std::mutex&,
            std::condition_variable&);
    ~WorkThread();

    std::thread* getThread();
    void wThread();
    void stop();
    void start();
    bool isRunning();

};

#endif /* WORKTHREAD_H_ */
