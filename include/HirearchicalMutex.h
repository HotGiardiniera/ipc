#pragma once
#include <mutex>
#include <climits>


namespace muts {
// TODO see this with gdb in a thread with multiple locks
    class HirearchicalMutex {
    public:
        explicit HirearchicalMutex(unsigned long value)
                : hierarchy_value(value),
                  previous_hierarchy_value(0) {}

        void lock() {
            check_for_hirearchy_violation();
            internal_mutex.lock();
            update_hirearchy_value();
        }

        void unlock() {
            if (this_thread_hirearchy_value != hierarchy_value) {
                throw std::logic_error("mutex hirearchy violated");
            }
            this_thread_hirearchy_value = previous_hierarchy_value;
            internal_mutex.unlock();
        }

        bool try_lock() {
            check_for_hirearchy_violation();
            if (!internal_mutex.try_lock())
                return false;
            update_hirearchy_value();
            return true;
        }

    private:
        std::mutex internal_mutex;
        unsigned long const hierarchy_value;
        unsigned long previous_hierarchy_value;
        static inline thread_local unsigned long this_thread_hirearchy_value{ULONG_MAX};

        void check_for_hirearchy_violation() {
            if (this_thread_hirearchy_value <= hierarchy_value) {
                throw std::logic_error("mutex hirearchy violated");
            }
        }

        void update_hirearchy_value() {
            previous_hierarchy_value = this_thread_hirearchy_value;
            this_thread_hirearchy_value = hierarchy_value;
        }
    };
}
