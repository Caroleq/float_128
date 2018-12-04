#ifndef SUBSTRACTION_TEST_H
#define SUBSTRACTION_TEST_H

#include <gtest/gtest.h>
#include "../src/float_128.h"


class substraction_test: public::testing::Test {
    
protected:
    virtual void SetUp() {
        
        
    }
    
    virtual void TearDown() {
        
    }

};


TEST_F( substraction_test, substract_floats_from_ints ){
    
    float_128 f1(6);
    float_128 f2(-3);
    float_128 f3(100);
    float_128 f4(-2399);
    float_128 f5(400000);
    
    
    float_128 f6 = f2 - f1;
    float_128 f7 = f4 - f5;
    float_128 f8 = f1 - f2;
    float_128 f9 = f5 - f4;

    
    ASSERT_EQ( f6.binary_representation(), "1 1000000000010 001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f7.binary_representation(), "1 1000000010001 100010001111011111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f8.binary_representation(), "0 1000000000010 001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f9.binary_representation(), "0 1000000010001 100010001111011111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
}


#endif