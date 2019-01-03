#ifndef COMPARISON_TEST_H
#define COMPARISON_TEST_H


#include <gtest/gtest.h>


TEST( comparison_test, trival )
{
    
    float_128 f1(6);
    float_128 f2(6.0);
    
    
    float_128 f3(9.0);
    float_128 f4(9);
    
    float_128 f5(-100000);
    float_128 f6(-100000.0);
    
    ASSERT_LE( f1, f2 );
    ASSERT_LE( f3, f4 );
    ASSERT_LE( f5, f6 );
    
}


TEST( comparison_test, simple )
{
    
    float_128 f1(6);
    float_128 f2(3.0);
    float_128 f3(9.0);
    float_128 f4(-100000);
    
    ASSERT_LE( f4, f2 );
    ASSERT_LE( f1, f3 );
    ASSERT_LE( f2, f3 );
    ASSERT_LE( f2, f1 );
}


#endif
