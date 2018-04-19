//
//  pingpongCondVar.cpp
//  Multithreading
//
//  Created by zaurilla on 19.04.2018.
//  Copyright © 2018 zaurilla. All rights reserved.
//

#include <iostream>
#include <condition_variable>
#include <thread>

bool ready{true};
std::condition_variable condVar;
int data = 0;
std::mutex mymutex;
std::mutex writemutex;

void pingSender1()
{
    while(data < 50) { // DATARACE
        std::unique_lock<std::mutex> lock(mymutex);
        condVar.wait(lock, []{return ready;});
        std::cout << (data%2?"ping":"pong") << std::endl;
        {
            std::unique_lock<std::mutex> lock(writemutex);
            ready = false;
        }
        
        data++;
        
        {
            std::unique_lock<std::mutex> lock(writemutex);
            ready = true;
        }
        condVar.notify_one();
    }
}

void pingSender2()
{
    while(data < 50) {
        std::unique_lock<std::mutex> lock(mymutex);
        condVar.wait(lock, []{return ready;});
        std::cout << (data%2?"ping":"pong") << std::endl;
        
        {
            std::unique_lock<std::mutex> lock(writemutex);
            ready = false;
        }
        
        data++;
        
        {
            std::unique_lock<std::mutex> lock(writemutex);
            ready = true;
        }
        condVar.notify_one();
    }
}

int main()
{
    std::cout << std::endl;
    
    std::thread t1(pingSender1);
    std::thread t2(pingSender2);
    
    t1.join();
    t2.join();
    
    std::cout << "\n\n";
    return 0;
}
