#include <gtest/gtest.h>
#include "../src/float_128.h"


class int_test: public::testing::Test {
    
protected:
    virtual void SetUp() {
        
        
    }
    
    virtual void TearDown() {
        
    }

};


TEST_F( int_test, comare_binary_representation ){
    
    float_128 f1(6);
    float_128 f2(-3);
    float_128 f3(100);
    float_128 f4(-2399);
    
    ASSERT_EQ( f1.binary_representation(), "0 1000000000001 100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f2.binary_representation(), "1 1000000000000 100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f3.binary_representation(), "0 1000000000101 100100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    ASSERT_EQ( f4.binary_representation(), "1 1000000001010 001010111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
}
