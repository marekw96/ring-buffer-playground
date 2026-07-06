#pragma once

#include <mutex>

template <typename T, size_t Size>
class mutex_ring_buffer {
public:
    bool put(const T& element_to_put) {
        std::lock_guard lg(mtx);

        if(((write_pos + 1) % (Size+1)) == read_pos) {
            return false;
        }

        elements[write_pos] = element_to_put;
        write_pos = (write_pos + 1) % (Size+1);
        return true;
    }

    std::optional<T> get() {
        std::lock_guard lg(mtx);

        if(write_pos != read_pos) {
            auto idx = read_pos;
            read_pos = (read_pos + 1) % (Size+1);

            return elements[idx];
        }

        return std::nullopt;
    }

private:
    std::mutex mtx;
    size_t write_pos = 0;
    size_t read_pos = 0;
    T elements[Size]{};
};