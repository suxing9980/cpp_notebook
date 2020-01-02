//
// Created by suxing on 2020/1/2.
//

#ifndef SYNCQUEUE_SYNCQUEUE_HPP
#define SYNCQUEUE_SYNCQUEUE_HPP

#include <list>
#include <thread>
#include <mutex>
#include <iostream>
#include <condition_variable>


template<typename T>
class SyncQueue
{
private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_notFull;
    int m_maxSize;
    bool m_needStop;

public:
    explicit SyncQueue(int maxSize):m_maxSize {maxSize}, m_needStop {false}
    {

    }
    void Put(const T& t)
    {
        Add(t);
    }

    void Put(T&& t)
    {
        Add(std::forward<T>(t));
    }

    void Take(std::list<T>& list)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this](){ return m_needStop || NotEmpty();});

        if(m_needStop)
        {
            return;
        }
        list = std::move(m_queue);
        m_notFull.notify_one();
    }

    void Take(T& t)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this](){return m_needStop || NotEmpty();});
        if(m_needStop)
        {
            return;
        }
        t = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }

    void Stop()
    {
        {
            std::unique_lock<std::mutex> locker(m_mutex);
            m_needStop = true;
        }
        m_notFull.notify_all();
        m_notEmpty.notify_all();
    }

    bool Empty()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }

    bool Full()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        return m_queue.size() == m_maxSize;
    }

    size_t Size()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

    int Count()
    {
        return Size();
    }

private:
    bool NotFull() const
    {
        bool full = m_queue.size() >= m_maxSize;
        if (full)
        {
            std::cout<<"缓冲区满了需要等待……"<<std::endl;
        }
        return !full;
    }

    bool NotEmpty() const
    {
        bool empty = m_queue.empty();
        if(empty)
        {
            std::cout<<"缓冲区空了，需要等待……，异步层的线程ID："<<std::this_thread::get_id()<<std::endl;
        }
        return !empty;
    }

    template<typename F>
    void Add(F&& x)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notFull.wait(locker, [this](){ return m_needStop || NotEmpty();});
        if(m_needStop)
            return;
        m_queue.push_back(std::forward<F>(x));
        m_notFull.notify_one();
    }
};


#endif //SYNCQUEUE_SYNCQUEUE_HPP
