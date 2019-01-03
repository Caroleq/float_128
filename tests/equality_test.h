#ifndef EQUALITY_TEST_H
#define EQUALITY_TEST_H


#include <gtest/gtest.h>


TEST( equality_test, trival )
{
    
    float_128 f1(6);
    float_128 f2(6.0);
    
    
    float_128 f3(9.0);
    float_128 f4(9);
    
    float_128 f5(-100000);
    float_128 f6(-100000.0);
    
    ASSERT_EQ( f1, f2 );
    ASSERT_EQ( f3, f4 );
    ASSERT_EQ( f5, f6 );
    
}


TEST( equality_test, simple )
{
    
    float_128 f1(6);
    float_128 f2(3.0);
    float_128 f3(9.0);
    
    float_128 f4(9.0);
    float_128 f5(-100000);
    float_128 f6(-100000.0+9);
    
    ASSERT_EQ( f1 + f2, f3 );
    ASSERT_EQ( f5 + f3, f6 );
    
}


#endif
