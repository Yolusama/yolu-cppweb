#pragma once

#include<thread>
#include<mutex>
#include<functional>
#include<queue>
#include<condition_variable>
#include<Windows.h>

namespace CppWeb {
    class ThreadPool
    {
    private:
        std::vector<std::thread> threads;
        std::queue<std::function<void()>>tasks;
        std::mutex mutex;
        std::condition_variable condition;
        bool shutdown;
        static const int MaxCount;
        static ThreadPool* pool;
        ThreadPool() { }
    public:
        static void Init(){
            pool = new ThreadPool();
            pool->shutdown = false;
            for (int i = 0; i < MaxCount; i++) {
                pool->threads.emplace_back([] {
                    while (true) {
                        std::unique_lock<std::mutex> lock(pool->mutex);
                        pool->condition.wait(lock, [] { return pool->shutdown || !pool->tasks.empty(); });
                        if (pool->shutdown && pool->tasks.empty()) {
                            return;
                        }
                        std::function<void()> task = std::move(pool->tasks.front());
                        pool->tasks.pop();
                        lock.unlock();
                        task();
                    }
                    });
            }
        }
        static void Add(std::function<void()>&& func)
        {
            {
                std::unique_lock<std::mutex> lock(pool->mutex);
                pool->tasks.push(func);
            }
            pool->condition.notify_one();
        }

        static const int MaxThreadCount() {
            return MaxCount;
        }

        static void Destroy() {
            {
                std::unique_lock<std::mutex> lock(pool->mutex);
                pool->shutdown = true;
            }
            pool->condition.notify_all();
            for (int i = 0; i < pool->threads.size(); i++) {
                pool->threads[i].join();
            }
            delete pool;
        }

        ~ThreadPool() { if(pool->threads.size()>0)Destroy(); }
    };
    ThreadPool* ThreadPool::pool;
    const int ThreadPool::MaxCount = std::thread::hardware_concurrency();
}