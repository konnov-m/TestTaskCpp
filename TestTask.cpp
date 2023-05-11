#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <time.h>
#include <stdlib.h>
#include <chrono>

std::mutex mutex_;

int countBitInInt(unsigned int num) {
    int count = 0;

    for (int i = 0; i < sizeof(unsigned int)*8; ++i) {
        if (((num >> i) & 1)) { 
            count++;
        }
    }
    
    return count;
}

void countBitInList(std::list<unsigned int>& list, bool direction) {
    int count = 0;
    int size = 0;
    int bit;
    int value;

    if (direction) {
        bit = 1;
    }
    else {
        bit = 0;
    }

    while (true) {
        mutex_.lock();

        if (list.empty()) {
            std::cout << "Counted " << count << " of " << direction << " size = " << size << std::endl;
            mutex_.unlock();
            return;
        }
        
        if (direction) {
            value = list.front();
            list.pop_front();
        }
        else {
            value = list.back();
            list.pop_back(); 
        }
        size++;
        mutex_.unlock();

        if (direction) {
            count += countBitInInt(value);
        }
        else {
            count += sizeof(unsigned int) * 8 -  countBitInInt(value);
        }
        
    }
}

int main() {
    std::list<unsigned int> list;


    for (int j = 0; j < 1; j++)
    {   
        std::cout << "Test # " << j + 1 << "\n";

        
        srand(time(NULL));


        for (int i = 0; i < 1000; i++) {
            list.push_back(rand() % 10);
        }


        std::thread thread1(countBitInList, std::ref(list), true);
        std::thread thread2(countBitInList, std::ref(list), false);


        thread1.detach();
        thread2.detach();

    } 
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
