//
// Created by suxing on 2020/1/2.
//

#ifndef SYNCQUEUE_SYNCQUEUE_H
#define SYNCQUEUE_SYNCQUEUE_H

#include <list>
#include <mutex>
#include <condition_variable>


template<typename T>
class SyncQueue
{
private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_noyFull;
    int m_maxSize;
    bool m_needStop;

public:
    explicit SyncQueue(int maxSize);
    void put(const T& t);
};


#endif //SYNCQUEUE_SYNCQUEUE_H
