#include <gtest/gtest.h>
#include <thread>

#include "no_lock_ring_buffer.hpp"

TEST(SPSC, sum_ones) {
    constexpr auto NUM_ELEMENTS = 10000;
    no_lock_ring_buffer<int, 5> rb;
    unsigned sum = 0;

    std::thread producer([&rb](){
        for(auto i = 0u; i < NUM_ELEMENTS; ++i){
            while(!rb.put(1)){std::this_thread::yield();}
        }
    });
    std::thread customer([&rb, &sum](){
        for(auto i = 0u; i < NUM_ELEMENTS; ++i){
            auto el = rb.get();
            while(!el)
                el = rb.get();

            sum += *el;
        }
    });

    producer.join();
    customer.join();

    ASSERT_EQ(sum, NUM_ELEMENTS);
}