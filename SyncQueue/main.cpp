#include <iostream>
#include "SyncQueue.hpp"
#include "ThreadPool.h"

int main()
{
    ThreadPool pool;
    pool.Start(2);

    std::thread thd1([&pool](){
        for(int i=0;i<10; ++i)
        {
            auto thdId = std::this_thread::get_id();
            pool.AddTask([thdId](){std::cout<<"同步层线程1的线程ID："<<thdId<<std::endl;});
        }
    });
    std::thread thd2([&pool](){
        for(int i=0;i<10; ++i)
        {
            auto thdId = std::this_thread::get_id();
            pool.AddTask([thdId](){std::cout<<"同步层线程2的线程ID："<<thdId<<std::endl;});
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(2));
    getchar();
    pool.Stop();
    thd1.join();
    thd2.join();
    return 0;
}