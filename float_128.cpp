#include "float_128.h"

void display_array( uint64_t array[] );


float_128 float_128::add_absolute_values( float_128 & float_to_add )
{
    
    int exp1 = get_exponent();
    int exp2 = float_to_add.get_exponent();
    
    uint64_t mantissa1[2];
    uint64_t mantissa2[2];
    uint64_t result[2];
    
    mantissa1[0] = bits[0]; 
    mantissa1[1] = bits[1];
    
    mantissa2[0] = float_to_add.bits[0]; 
    mantissa2[1] = float_to_add.bits[1];
    
    mantissa1[0] = ( mantissa1[0] << 14 ) >> 14;
    mantissa2[0] = ( mantissa2[0] << 14 ) >> 14;
    
    
    int one1 = 1;
    int one2 = 1;

    
    int exp = 0;
    
    if( exp1 > exp2 ){
        exp = exp1;
        one2 = 0;
        shift_bits_in_array(mantissa2, exp1-exp2);
    }
    else if( exp1 < exp2 ){
        exp = exp2;
        one1 = 0;
        shift_bits_in_array(mantissa1, exp2-exp1);
    }
    else{
        exp = exp1 + 1;
        shift_bits_in_array(mantissa1, 1);
        shift_bits_in_array(mantissa2, 1);
        
        mantissa1[0] &= ~(1ULL << 50 );
        mantissa2[0] &= ~(1ULL << 50 );
    }

    int accumulator = 0;

  /*  std::cout << "matisissa1: " << std::endl;
    display_array(mantissa1);
    std::cout << "matisissa2: " << std::endl;
    display_array(mantissa2);    */
    
    result[0] = 0;
    result[1] = 0;
    

    for( int i=0; i<64; i++){
         
            bool bit1 = ( mantissa1[1] >> i ) & 1; 
            bool bit2 = ( mantissa2[1] >> i ) & 1; 
            
            accumulator += bit1 + bit2;
            
            if( (accumulator%2) == 1 )
                result[1] = result[1] | ( 1ULL << i );
            
            if( accumulator > 1)
                accumulator = accumulator >> 1;
            else
                accumulator = 0;
        }
        

        
    for( int i=0; i< 50; i++){
         
            bool bit1 = ( mantissa1[0] >> i ) & 1; 
            bool bit2 = ( mantissa2[0] >> i ) & 1; 
            
            accumulator += bit1 + bit2;

            if( (accumulator%2) == 1 ){
                result[0] = result[0] | ( 1ULL << i );
            }
            
            if( accumulator > 1)
                accumulator = accumulator / 2;
            else
                accumulator = 0;
        }
        


    if( accumulator + one1 + one2  == 2)
        exp++;
    
    
    if( is_negative() )
        result[0] = result[0] | (1ULL <<63);
    
    
    float_128 resultt;
    resultt.bits[0] = result[0];
    resultt.bits[1] = result[1];
    
    resultt.set_exponent(exp+4096);
    
  
    
    return resultt;
    
}


void float_128::shift_bits_in_array( uint64_t array [], int shift )
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


