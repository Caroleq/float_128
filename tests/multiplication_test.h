#ifndef MULTIPLICATION_TEST_H
#define MULTIPLICATION_TEST_H

#include <gtest/gtest.h>
#include "../src/float_128.h"


class multiplication_test: public::testing::Test {
    
protected:
    virtual void SetUp() {
        
        
    }
    
    virtual void TearDown() {
        
    }

};


TEST_F( multiplication_test, multiplcation ){
    
    float_128 f1(6);
    float_128 f2(-3);
    float_128 f3(100);
    float_128 f4(-2399);
    float_128 f5(400000);
    float_128 f6(2);
    
    float_128 f7 = f1 * f2;
    
    ASSERT_EQ( f7.binary_representation(), "1 1000000000011 001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
}



#endif
