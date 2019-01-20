#ifndef VECTOR_TEST_H
#define VECTOR_TEST_H


#include <gtest/gtest.h>

#include "../src/float_128.h"

extern void divide_two( std::vector<char> & vec );
extern void add_bit( std::vector<char> & dec_representation, int index );
extern void mantissa_to_decimal( std::vector<char> & dec_representation, const float_128 float_to_print );
extern void multiply_twice( std::vector<char> & vec );
extern void divide_twice( std::vector<char> & vec );

TEST( divide_two_test, simple )
{
    
    std::vector<char> vec1{1};
    
    divide_two( vec1 );
    ASSERT_EQ( vec1, (std::vector<char>{0,5}) );
    
    divide_two( vec1 );
    ASSERT_EQ( vec1, (std::vector<char>{0,2,5}) );
    
    divide_two( vec1 );
    ASSERT_EQ( vec1, (std::vector<char>{0,1,2,5}) );
    
    divide_two( vec1 );
    ASSERT_EQ( vec1, (std::vector<char>{0,0,6,2,5}) );
    
    ASSERT_NE( vec1, (std::vector<char>{0,0,6,2,8}) );
}



TEST( add_new_bit_test, simple )
{
    
    std::vector<char> vec1{1};
    
    add_bit( vec1,  4 );
    ASSERT_EQ( vec1, (std::vector<char>{1, 0, 6, 2, 5}) );
    
    add_bit( vec1,  2 );
    ASSERT_EQ( vec1, (std::vector<char>{1, 3, 1, 2, 5}) );
    
    add_bit( vec1, 1 );
    ASSERT_EQ( vec1, (std::vector<char>{1, 8, 1, 2, 5}) );
    
}



TEST( get_mantissa_test, simple )
{
   float_128 f1("01000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   float_128 f2("01000000000001100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   float_128 f3("01000000000001110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   float_128 f4("01000000000001010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   std::vector<char> res;
   
   mantissa_to_decimal( res, f1 );
   ASSERT_EQ( res, (std::vector<char>{1, '.', 5}) );
   
   mantissa_to_decimal( res, f2 );
   ASSERT_EQ( res, (std::vector<char>{1, '.', 5}) );
   
   mantissa_to_decimal( res, f3 );
   ASSERT_EQ( res, (std::vector<char>{1, '.', 7, 5}) );
   
   mantissa_to_decimal( res, f4 );
   ASSERT_EQ( res, (std::vector<char>{1, '.', 2, 5}) );

}

TEST( multiply_twice_test, simple )
{
   float_128 f1("01000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   float_128 f2("01000000000001010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   float_128 f3("01000000000001110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   std::vector<char> res;
   
   mantissa_to_decimal( res, f1 );
   multiply_twice( res );
   ASSERT_EQ( res, (std::vector<char>{3}) );
   
   mantissa_to_decimal( res, f2 );
   multiply_twice( res );
   ASSERT_EQ( res, (std::vector<char>{2, '.', 5}) );
   
   mantissa_to_decimal( res, f3 );
   multiply_twice( res );
   ASSERT_EQ( res, (std::vector<char>{3, '.', 5}) );

}


TEST( divide_twice_test, simple )
{
   float_128 f1("01000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   float_128 f2("01000000000001010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   float_128 f3("01000000000001110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");   
   std::vector<char> res;
   
   mantissa_to_decimal( res, f1 );
   ASSERT_EQ( res, (std::vector<char>{1, '.', 5}) );
   divide_twice( res );
   ASSERT_EQ( res, (std::vector<char>{0,'.', 7,5}) );
   
   mantissa_to_decimal( res, f2 );
   divide_twice( res );
   ASSERT_EQ( res, (std::vector<char>{0, '.', 6, 2, 5}) );
   
   mantissa_to_decimal( res, f3 );
   divide_twice( res );
   ASSERT_EQ( res, (std::vector<char>{0, '.', 8, 7, 5}) );

}

#endif
