#include <iostream>
#include <shared_mutex>
#include <thread>

std::shared_mutex mutex;
int shared_data = 0;

void reader() {
    std::shared_lock<std::shared_mutex> lock(mutex);
    std::cout << "Read: " << shared_data << std::endl;
}

void writer() {
    std::unique_lock<std::shared_mutex> lock(mutex);
    shared_data++;
    std::cout << "Write: " << shared_data << std::endl;
}

int main() {
    std::thread t1(reader);
    std::thread t2(writer);
    t1.join();
    t2.join();
    return 0;
}


