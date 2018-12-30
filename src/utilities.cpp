#include <cstdint>
#include <iostream>

#include "exceptions/invalid_index.h"


int get_bit( uint64_t bit_arr[], int index );
void shift_bits_in_array_left( uint64_t array[] , int positions );
void shift_bits_in_array_right( uint64_t array [], int shift );
void display_array( uint64_t array[] );
void set_array( uint64_t bits1[], int arr1[], bool set_1, uint64_t bits2[], int arr2[], bool set_2);
int convert_to_mantissa( uint64_t mantissa[], int arr[] );
void multiply_mantissas( uint64_t mantissa1[], uint64_t mantissa2[], int result [] );



void shift_bits_in_array_left( uint64_t array[] , int positions )
{
    
    if( positions < 64 ){
            
        uint64_t higher = ( array[0] << positions ) | (array[1] >>(64-positions));
        uint64_t lower = ( array[1] << positions );
        
        array[0] = higher;
        array[1] = lower;
    }
    else if( positions == 64 ) {
            array[0] = array[1];
            array[1] = 0;
    }
    else {
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
    
    array[0] = high;
    array[1] = less;
    
}



void display_array( uint64_t array[] )
{
    
    for( int i=7; i>-1; i--){
      for( int j=7; j>-1; j--){
          std::cout << ( ( array[0] >> (8*i+j) ) & 1 );
          if( ( i==7 && j==7 ) || ( i==6 && j==2) )
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
    while( (! arr[to_shift]) && to_shift<114) to_shift++;
    
    
    mantissa[0] = 0;
    mantissa[1] = 0;
    
    if( to_shift == 114 && arr[to_shift] == 0 )
        return -1;
    
    for( int i=14; i<64 &&  i+to_shift <128; i++){
            
        if( arr[i+to_shift-13] )
            mantissa[0] = mantissa[0] | ( 1ULL << (63-i) ) ;
            
    }
    
    for( int i=0; i<64 && i+to_shift <64; i++){
        
        if( arr[i+to_shift+50] )
            mantissa[1] = mantissa[1] | ( 1ULL << (63-i) );
    }
    
    return to_shift;
}


int get_bit( uint64_t bit_arr[], int index )
{
    if( index < 0 || index > 128 )
        throw invalid_index_error("Index should be in range [0..127] " + std::to_string(index) + " provided");
    
    if( index > 63 ){
        
        index -= 64;
        
        return ( bit_arr[0] >> index ) & 1;
    }
    
    return ( bit_arr[1] >> index ) & 1;
    
}

void multiply_mantissas( uint64_t mantissa1[], uint64_t mantissa2[], int result [] )
{
    mantissa1[0] = ( mantissa1[0] << 14 ) >> 14;
    mantissa2[0] = ( mantissa2[0] << 14 ) >> 14;
    
    mantissa1[0] |= 1ULL << 50;
    mantissa2[0] |= 1ULL << 50;
    
    
    for( int i=0; i< 230; i++)
        result[i] = 0;
    
    for( int i=0; i < 115; i++ ){
        for( int j=0; j < 115; j++){
            
            int bit1 = get_bit( mantissa1, i );
            int bit2 = get_bit( mantissa2, j);
            
            result[ 229 - j - i ] += bit1 * bit2;
        }
    }
    
    
    for( int i=229; i>0; i--){
        int val = result[i];
        result[i] = val % 2;
        result[i-1] += val / 2;
    }
    
    
}
