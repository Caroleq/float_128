#include <iostream>
#include "float_128.h"

int main()
{
    
    float_128 float0(4);
    std::cout << float0.binary_representation() << std::endl;
    
    
    float_128 float1(2.3);
    std::cout << float1.binary_representation() << std::endl;
    
    
    float_128 float2(34.88);
    std::cout << float2.binary_representation() << std::endl;
    
  return 0;   
}
