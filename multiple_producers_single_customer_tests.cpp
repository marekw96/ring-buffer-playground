#include <gtest/gtest.h>
#include <thread>

#include "mutex_ring_buffer.hpp"

TEST(MPSC, sum_ones) {
    constexpr auto NUM_ELEMENTS = 1000;
    mutex_ring_buffer<int, 5> rb;
    unsigned sum = 0;

    auto producer = [&rb](){
        for(auto i = 0u; i < NUM_ELEMENTS; ++i){
            while(!rb.put(1)){}
        }
    };

    std::thread producer_1(producer);
    std::thread producer_2(producer);
    std::thread customer([&rb, &sum](){
        for(auto i = 0u; i < 2 * NUM_ELEMENTS; ++i){
            auto el = rb.get();
            while(!el)
                el = rb.get();

            sum += *el;
        }
    });

    producer_1.join();
    producer_2.join();
    customer.join();

    ASSERT_EQ(sum, 2 * NUM_ELEMENTS);
}