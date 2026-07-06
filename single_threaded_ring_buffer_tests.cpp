#include <gtest/gtest.h>

#include "no_lock_ring_buffer.hpp"
#include "mutex_ring_buffer.hpp"

template <typename ring_buffer_type>
struct ST : ::testing::Test{
    ring_buffer_type rb;
};

using testing::Types;

using implementations = Types<no_lock_ring_buffer<int, 5>,
                              mutex_ring_buffer<int, 5>>;

TYPED_TEST_SUITE(ST, implementations);

TYPED_TEST(ST, nullopt_on_empty_rb_get){
    ASSERT_EQ(this->rb.get(), std::nullopt);
}

TYPED_TEST(ST, put_on_empty_rb_should_success){
    ASSERT_TRUE(this->rb.put(99));
}

TYPED_TEST(ST, put_on_full_rb_should_fail){
    ASSERT_TRUE(this->rb.put(90));
    ASSERT_TRUE(this->rb.put(91));
    ASSERT_TRUE(this->rb.put(92));
    ASSERT_TRUE(this->rb.put(93));
    ASSERT_TRUE(this->rb.put(94));
    ASSERT_FALSE(this->rb.put(95));
}

TYPED_TEST(ST, put_get) {
    ASSERT_TRUE(this->rb.put(90));
    ASSERT_EQ(*this->rb.get(), 90);
}

TYPED_TEST(ST, fill_whole_then_get) {
    ASSERT_TRUE(this->rb.put(90));
    ASSERT_TRUE(this->rb.put(91));
    ASSERT_TRUE(this->rb.put(92));
    ASSERT_TRUE(this->rb.put(93));
    ASSERT_TRUE(this->rb.put(94));

    ASSERT_EQ(*this->rb.get(), 90);
    ASSERT_EQ(*this->rb.get(), 91);
    ASSERT_EQ(*this->rb.get(), 92);
    ASSERT_EQ(*this->rb.get(), 93);
    ASSERT_EQ(*this->rb.get(), 94);
}

TYPED_TEST(ST, check_overflow) {
    ASSERT_TRUE(this->rb.put(90));
    ASSERT_TRUE(this->rb.put(91));
    ASSERT_TRUE(this->rb.put(92));
    ASSERT_TRUE(this->rb.put(93));
    ASSERT_TRUE(this->rb.put(94));

    ASSERT_EQ(*this->rb.get(), 90);
    ASSERT_EQ(*this->rb.get(), 91);

    ASSERT_TRUE(this->rb.put(95));
    ASSERT_TRUE(this->rb.put(96));

    ASSERT_EQ(*this->rb.get(), 92);
    ASSERT_EQ(*this->rb.get(), 93);
    ASSERT_EQ(*this->rb.get(), 94);
    ASSERT_EQ(*this->rb.get(), 95);
    ASSERT_EQ(*this->rb.get(), 96);
}