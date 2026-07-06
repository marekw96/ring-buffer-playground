#include <gtest/gtest.h>

#include "no_lock_ring_buffer.hpp"


TEST(single_thread, nullopt_on_empty_rb_get){
    no_lock_ring_buffer<int, 5> rb;

    ASSERT_EQ(rb.get(), std::nullopt);
}

TEST(single_thread, put_on_empty_rb_should_success){
    no_lock_ring_buffer<int, 5> rb;

    ASSERT_TRUE(rb.put(99));
}

TEST(single_thread, put_on_full_rb_should_fail){
    no_lock_ring_buffer<int, 5> rb;

    ASSERT_TRUE(rb.put(90));
    ASSERT_TRUE(rb.put(91));
    ASSERT_TRUE(rb.put(92));
    ASSERT_TRUE(rb.put(93));
    ASSERT_TRUE(rb.put(94));
    ASSERT_FALSE(rb.put(95));
}

TEST(single_thread, put_get) {
    no_lock_ring_buffer<int, 5> rb;

    ASSERT_TRUE(rb.put(90));
    ASSERT_EQ(*rb.get(), 90);
}

TEST(single_thread, fill_whole_then_get) {
    no_lock_ring_buffer<int, 5> rb;

    ASSERT_TRUE(rb.put(90));
    ASSERT_TRUE(rb.put(91));
    ASSERT_TRUE(rb.put(92));
    ASSERT_TRUE(rb.put(93));
    ASSERT_TRUE(rb.put(94));

    ASSERT_EQ(*rb.get(), 90);
    ASSERT_EQ(*rb.get(), 91);
    ASSERT_EQ(*rb.get(), 92);
    ASSERT_EQ(*rb.get(), 93);
    ASSERT_EQ(*rb.get(), 94);
}

TEST(single_thread, check_overflow) {
    no_lock_ring_buffer<int, 5> rb;

    ASSERT_TRUE(rb.put(90));
    ASSERT_TRUE(rb.put(91));
    ASSERT_TRUE(rb.put(92));
    ASSERT_TRUE(rb.put(93));
    ASSERT_TRUE(rb.put(94));

    ASSERT_EQ(*rb.get(), 90);
    ASSERT_EQ(*rb.get(), 91);

    ASSERT_TRUE(rb.put(95));
    ASSERT_TRUE(rb.put(96));

    ASSERT_EQ(*rb.get(), 92);
    ASSERT_EQ(*rb.get(), 93);
    ASSERT_EQ(*rb.get(), 94);
    ASSERT_EQ(*rb.get(), 95);
    ASSERT_EQ(*rb.get(), 96);
}