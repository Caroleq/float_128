#ifndef FROM_DOUBLE_TEST_H
#define FROM_DOUBLE_TEST_H

#include <gtest/gtest.h>
#include "../src/float_128.h"



TEST( from_double_construction_test, compare_binary_representation )
{
    
    float_128 f1(2.3);
    float_128 f2(3.99);
    float_128 f3(345.5);
    float_128 f4(-87.32423);
    float_128 f5(0.00000000002);
    
    ASSERT_EQ( f1.binary_representation(), "0 1000000000000 001001100110011001100110011001100110011001100110011000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f2.binary_representation(), "0 1000000000000 111111101011100001010001111010111000010100011110110000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f3.binary_representation(), "0 1000000000111 010110011000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f4.binary_representation(), "1 1000000000101 010111010100110000000010111100101111100110000111010000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f5.binary_representation(), "0 0111111011011 010111111101011111111110000101111001011001001001010100000000000000000000000000000000000000000000000000000000000000" );
}



#endif
