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
 //   high = high | ( 1ULL << ( 50 -shift) );
    
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


void set_array( uint64_t bits1[], int arr1[], bool set_1, uint64_t bits2[], int arr2[], bool set_2)
{
    if( set_1 )
        arr1[0] = 1;
    else
        arr1[0] = 0;
    
    if( set_2 )
        arr2[0] = 1;
    else
        arr2[0] = 0;
    
    for( int i=0; i<50; i++){
        arr1[i+1] = ( bits1[0] >> (49-i) ) & 1;
        arr2[i+1] = ( bits2[0] >> (49-i) ) & 1;
    }
    
    for( int i=0; i<64; i++){
        arr1[i+1+50] = ( bits1[1] >> (63-i) ) & 1;
        arr2[i+1+50] = ( bits2[1] >> (63-i) ) & 1;
    }
    
}


int convert_to_mantissa( uint64_t mantissa[], int arr[] ){
    
    int to_shift = 0;
    while( ! arr[to_shift] ) to_shift++;
    
    mantissa[0] = 0;
    mantissa[1] = 0;
    
    for( int i=14; i<64 &&  i+to_shift+1-14 <64+1+64; i++){
            
        if( arr[i+to_shift+1-14] )
            mantissa[0] = mantissa[0] | ( 1ULL << (63-i) ) ;
            
    }
    
    
    for( int i=0; i<64 && i+to_shift <64; i++){
        
        if( arr[i+to_shift+1-14+50] )
            mantissa[1] = mantissa[1] | ( 1ULL << (63-i) );
    }
    
    return to_shift;
}


