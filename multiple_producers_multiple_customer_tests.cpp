#include <gtest/gtest.h>
#include <thread>

#include "mutex_ring_buffer.hpp"
#include "atomic_ring_buffer.hpp"

template <typename ring_buffer_type>
struct MPMC : ::testing::Test{
    ring_buffer_type rb;
};

using testing::Types;

using implementations = Types<mutex_ring_buffer<int, 5>,
                              atomic_ring_buffer<int, 5>>;

TYPED_TEST_SUITE(MPMC, implementations);

TYPED_TEST(MPMC, Producers2_Customers2) {
    constexpr auto NUM_ELEMENTS = 10000;
    std::atomic<size_t> sum_producer{0};
    std::atomic<size_t> sum_customer{0};

    auto producer = [&rb = this->rb, &sum_producer](){
        size_t sum = 0;
        for(auto i = 0u; i < NUM_ELEMENTS; ++i){
            while(!rb.put(i)){}
            sum += i;
        }

        sum_producer.fetch_add(sum);
    };

    auto customer = [&rb = this->rb, &sum_customer](){
        size_t sum = 0;
        for(auto i = 0u; i < NUM_ELEMENTS; ++i){
            auto el = rb.get();
            while(!el)
                el = rb.get();

            sum += *el;
        }

        sum_customer.fetch_add(sum);
    };

    std::thread producer_1(producer);
    std::thread producer_2(producer);
    std::thread customer_1(customer);
    std::thread customer_2(customer);

    producer_1.join();
    producer_2.join();
    customer_1.join();
    customer_2.join();

    ASSERT_EQ(sum_producer.load(), sum_customer.load());
}

TYPED_TEST(MPMC, Producers4_Customers2) {
    constexpr auto NUM_ELEMENTS = 10000;
    std::atomic<size_t> sum_producer{0};
    std::atomic<size_t> sum_customer{0};

    auto producer = [&rb = this->rb, &sum_producer](){
        size_t sum = 0;
        for(auto i = 0u; i < NUM_ELEMENTS; ++i){
            while(!rb.put(i)){}
            sum += i;
        }

        sum_producer.fetch_add(sum);
    };

    auto customer = [&rb = this->rb, &sum_customer](){
        size_t sum = 0;
        for(auto i = 0u; i < 2 * NUM_ELEMENTS; ++i){
            auto el = rb.get();
            while(!el)
                el = rb.get();

            sum += *el;
        }

        sum_customer.fetch_add(sum);
    };

    std::thread producer_1(producer);
    std::thread producer_2(producer);
    std::thread producer_3(producer);
    std::thread producer_4(producer);
    std::thread customer_1(customer);
    std::thread customer_2(customer);

    producer_1.join();
    producer_2.join();
    producer_3.join();
    producer_4.join();
    customer_1.join();
    customer_2.join();

    ASSERT_EQ(sum_producer.load(), sum_customer.load());
}