#include <cstdint>
#include <iostream>

void shift_bits_in_array_left( uint64_t array[] , int positions )
{
    
    
    
    if( positions < 64 ){
            
        uint64_t higher = ( array[0] << positions ) | (array[1] >>(64-positions));
        uint64_t lower = ( array[1] << positions );
        
        array[0] = higher;
        array[1] = lower;
    }
    else if( positions == 64 ){
            array[0] = array[1];
            array[1] = 0;
    }
    else{
        // to check
        array[0] = array[1] << (128-positions);
        array[1] = 0;
    }
}


void shift_bits_in_array_right( uint64_t array [], int shift )
{

    uint64_t less = array[1] >> shift;
    
    uint64_t higher_to_lower = ( array[0] << (64-shift) );
    
    less = less | higher_to_lower;
    
    uint64_t high = array[0] >> shift;
    high = high | ( 1ULL << ( 50 -shift) );
    
    array[0] = high;
    array[1] = less;
    
}



void display_array( uint64_t array[] )
{
    
    for( int i=7; i>-1; i--){
      for( int j=7; j>-1; j--){
          std::cout << ( ( array[0] >> (8*i+j) ) & 1 );
          if( i==7 && j==7 || i==6 && j==2)
              std::cout << " ";
      }
    }
    
    for( int i=7; i>-1; i--){
      for( int j=7; j>-1; j--)
          std::cout << ( ( array[1] >> (8*i+j) ) & 1 );
    }
    
    std::cout << std::endl;
    
}


