#pragma once
#include <array>
#include <mutex>


template <typename T, size_t N>
class CircularBuffer {
public:
    CircularBuffer()
    {
        m_index_mask = N - 1;
        static_assert( N > 0, "Capacity must be greater than 0" );
        static_assert( (N & (N - 1)) == 0, "Capacity must be power of 2" );

    }

    auto mut_push(T const& value) -> void
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        const auto next_index = (m_write_loc + 1) & m_index_mask;
        if (next_index == m_read_loc)
        {
            throw std::out_of_range("Circular buffer overflow");
        }
        m_buffer[m_write_loc] = value;
        m_write_loc = next_index;
    }

    auto mut_pop() -> T
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_write_loc == m_read_loc)
        {
            throw std::out_of_range("Read is caught up with write");
        }
        auto value = m_buffer[m_read_loc];
        m_read_loc = (m_read_loc + 1) & m_index_mask;
        return value;
    }

private:
    // Lock based circular buff
    std::mutex m_mutex;

    uint64_t m_write_loc{ 0 };
    uint64_t m_read_loc{ 0 };


    size_t m_capacity = N;
    size_t m_index_mask{};
    std::array<T, N> m_buffer;
};

