#include <iostream>
#include "float_128.h"

int main()
{
    
    float_128 float0(4);
    //std::cout << float0.binary_representation() << std::endl;
    
    float_128 float1(2.3);
    
    float_128 float2(34.88);
    
    float_128 float3(2);
    
    float_128 float4 = float0 + float3;
    
    float_128 float5 = float0 + float1;
    
    std::cout << float4.binary_representation()  << std::endl;
    
    std::cout << float5.binary_representation()  << std::endl;
    
  return 0;   
}


