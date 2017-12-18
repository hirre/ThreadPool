/*
 * main.cpp
 *
 * Test program for ThreadPool.
 *
 *      Author: Hirad Asadi
 */

#define LOOP_COUNT 4

#include <iostream>
#include <unistd.h>

#include "../src/ThreadPool.h"

using namespace std;

void w()
{
    // Simulate 1 second of work
    sleep(1);
}

int main(int argc, char** argv)
{
    unsigned cores = std::thread::hardware_concurrency();
    cout << "std::thread::hardware_concurrency(): " << cores << endl;

    unsigned int c = 0;
    unsigned t_c;

    if (argc == 2)
        c = atoi(argv[1]);

    if (c <= cores)
        t_c = c;
    else
        t_c = 1;

    cout << "# selected threads: " << t_c << endl;
    cout << "Total threads: " << t_c + 1 << endl;
    cout << ">>> Starting work..." << endl;


    if (t_c > 0)
    {
        // Run work on other thread(s)
        try
        {
            ThreadPool tp(t_c);

            tp.start();
            tp.start(); // debug test

            for (int i = 0; i < LOOP_COUNT; ++i)
            {
                tp.addWork(&w);
            }

            tp.stop();
            tp.stop(); // debug test

            // Run other work
            tp.start();

            for (int i = 0; i < LOOP_COUNT; ++i)
            {
                tp.addWork(&w);
            }

            tp.stop();
        } catch (exception& e)
        {
            cerr << e.what() << endl;
        }
    }
    else
    {
        // Run work on this thread
        for (int i = 0; i < LOOP_COUNT*2; ++i)
        {
            w();
        }
    }

    cout << ">>> Work is complete." << endl;
    return 0;
}
