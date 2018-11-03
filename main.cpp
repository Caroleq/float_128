#include <iostream>
#include "float_128.h"

int main()
{
    
    float_128 float_(4);
    
   // std::cout << float_.get_bit( 0 ) << std::endl;
   // std::cout << float_.get_bit( 1 ) << std::endl;
   // std::cout << float_.get_bit( 2 ) << std::endl;
    
   // float_.set_bit( 7 );
    
   // std::cout << float_.get_bit( 2 ) << std::endl;
    
    std::cout << float_.binary_representation() << std::endl;
    
    
    float_128 float1(2.3);
    std::cout << float1.binary_representation() << std::endl;
    
  return 0;   
}
