#include <gtest/gtest.h>
#include "../src/utilities.cpp"



TEST(shift_test, shift_bits_in_array_left_function ){
    
    uint64_t to_shift[2];
    
    to_shift[0] = 6507006303128227321;      
    to_shift[1] = 999637616634902401;       
    
    shift_bits_in_array_left( to_shift , 10 );
    
    ASSERT_EQ( 3899843794156643383, to_shift[0] );
    ASSERT_EQ( 9057995380114719744, to_shift[1] );
    
}



TEST(shift_test, shift_bits_in_array_right_function ){
    
    uint64_t to_shift[2];
    
    to_shift[0] = 6507006303128227321; 
    to_shift[1] = 999637616634902401;       
    
    shift_bits_in_array_right( to_shift , 10 );
    
    ASSERT_EQ( 6354498342898659, to_shift[0] );
    ASSERT_EQ( 9098247455898396941, to_shift[1] );
    
}

