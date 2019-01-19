#ifndef VECTOR_TEST_H
#define VECTOR_TEST_H


#include <gtest/gtest.h>

#include "../src/float_128.h"

extern void divide_two( std::vector<char> & vec );
extern void add_bit( std::vector<char> & dec_representation, int index );
extern void mantissa_to_decimal( std::vector<char> & dec_representation, const float_128 float_to_print );

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
   std::vector<char> res;
   
 //  std::cout << f1.binary_representation() << std::endl;
   
   mantissa_to_decimal(res, f1 );
  /* for( int i=0; i<res.size(); i++)
       std::cout << char( res[i]+48) << " ";
   std::cout << std::endl;
   */
}

#endif
