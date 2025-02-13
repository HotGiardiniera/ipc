#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
//#include "ScopedThread.h"
#include "HirearchicalMutex.h"
#include <boost/asio.hpp>
#include "ipc/ipc.h"
#include "ipc/file_locking.h"
#include "ipc/queue.h"


int GLOBAL = 5;

class Test
{
public:
    void print_int(int &x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "thread " << std::this_thread::get_id() << " sleeping...\n";
        std::cout << "X is "<< x << std::endl;
    }
};

void func(int &var) {
    std::cout << "Var is "<< var << std::endl;
}

//void basic_threads()
//{
//    std::cout << "We can use this much concurrency " << std::thread::hardware_concurrency() << std::endl;
//    int local = 5;
//    Test test{};
//    std::cout << "thread " << std::this_thread::get_id() << " main\n";
//    ScopedThread st(std::thread (&Test::print_int, &test, std::ref(local)));
//    std::cout << "Finishing up" << std::endl;
//}

void locks(std::mutex& mut)
{
    std::lock_guard<std::mutex> guard(mut);

}

muts::HirearchicalMutex high_level_mutex(10000);
muts::HirearchicalMutex low_level_mutex(5000);

void edit_global_1(muts::HirearchicalMutex& mut)
{
    std::lock_guard<muts::HirearchicalMutex> guard(mut);
    std::this_thread::sleep_for(std::chrono::seconds(30));
    GLOBAL += 1;
    std::cout << "GLOBAL in 1 is " << GLOBAL << std::endl;

}

void edit_global_2(muts::HirearchicalMutex& mut)
{
    std::lock_guard<muts::HirearchicalMutex> guard(mut);
    std::this_thread::sleep_for(std::chrono::seconds(30));
    GLOBAL += 1;
    std::cout << "GLOBAL in 2 is " << GLOBAL << std::endl;

}



int main(int argc, char* argv[])
{
//    std::mutex my_mut{};
//
//    std::thread t1 = std::thread(edit_global_1, std::ref(high_level_mutex));
//    std::thread t2 = std::thread(edit_global_2, std::ref(low_level_mutex));
//
//
//    t1.join();
//    t2.join();

//    // Signals
//    signals();

//    // Pipes
//    pipes();

//    // Fifo has a reader and a writer, launch the reader in a separate thread
//    std::thread reader_thread = std::thread(fifo_read);
//    fifo_write();
//    reader_thread.join();

    // Queue has a reader and a writer, launch the reader in a separate thread
    std::thread reader_thread = std::thread(queue_receive);
    queue_send();
    reader_thread.join();




//    file_lock(argc, argv);


    return 0;
}
