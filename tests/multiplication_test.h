#ifndef MULTIPLICATION_TEST_H
#define MULTIPLICATION_TEST_H

#include <gtest/gtest.h>
#include "../src/float_128.h"



TEST( multiplication_test, ints_multiplcation ){
    
    float_128 f1(6);
    float_128 f2(-3);
    float_128 f3(100);
    float_128 f4(-2399);
    float_128 f5(400000);
    float_128 f6(2);
    
    float_128 f7 = f1 * f2;
    float_128 f8 = f2 * f1;
    float_128 f9 = f5 * f6;
    float_128 f10 = f6 * f5;
    float_128 f11 = f3 * f4;
    float_128 f12 = f3 * f4;
    
    ASSERT_EQ( f7.binary_representation(), "1 1000000000011 001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f8.binary_representation(), "1 1000000000011 001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f9.binary_representation(), "0 1000000010010 100001101010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f10.binary_representation(), "0 1000000010010 100001101010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f11.binary_representation(), "1 1000000010000 110101001000111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f12.binary_representation(), "1 1000000010000 110101001000111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
}




#endif
