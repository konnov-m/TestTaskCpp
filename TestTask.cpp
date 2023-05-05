#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <time.h>
#include <stdlib.h>

std::mutex mutex_;

int countBitInInt(int num, int bit) {
    int count = 0;

    if (bit != 0 && bit != 1) {
        return -1;
    }

    for (int i = 0; i < sizeof(int)*8; ++i) {
        if (bit == ((num >> i) & 1)) {
            count++;
        }
    }
    
    return count;
}

void countBitInList(std::list<int>& list, bool direction) {
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
        std::unique_lock<std::mutex> lock(mutex_);
        if (list.empty()) {
            std::cout << "Counted " << count << " of " << direction << " size = " << size << std::endl;
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
        lock.unlock();

        
        count += countBitInInt(value, bit);
    }
}

int main() {
    for (int j = 0; j < 1000; j++)
    {   
        std::cout << "Test # " << j + 1 << "\n";

        std::list<int> list;
        srand(time(NULL));


        for (int i = 0; i < 1000; i++) {
            list.push_back(rand() % 10);
        }


        std::thread thread1(countBitInList, std::ref(list), true);
        std::thread thread2(countBitInList, std::ref(list), false);


        thread1.join();
        thread2.join();

    }

    return 0;
}
