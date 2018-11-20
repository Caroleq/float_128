#include "float_128.h"

extern void display_array( uint64_t array[] );
extern void shift_bits_in_array_left( uint64_t array[] , int positions );
extern void shift_bits_in_array_right( uint64_t array [], int shift );

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
        shift_bits_in_array_right(mantissa2, exp1-exp2);
    }
    else if( exp1 < exp2 ){
        exp = exp2;
        one1 = 0;
        shift_bits_in_array_right(mantissa1, exp2-exp1);
    }
    else{
        exp = exp1 + 1;
        shift_bits_in_array_right(mantissa1, 1);
        shift_bits_in_array_right(mantissa2, 1);
        
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




bool float_128::leq_abs( float_128 & float_to_compare ){
  
    int exp1 = get_exponent();
    int exp2 = float_to_compare.get_exponent();
    
    if( exp1 < exp2 )
        return 1;
    if( exp1 > exp2)
        return 0;
    
    uint64_t mantissa1 = ( bits[0] >> 14 ) << 14;
    uint64_t mantissa2 = ( float_to_compare.bits[0] >> 14 ) << 14;
    
    if( mantissa1 < mantissa2 )
        return 1;
    
    if( mantissa1 > mantissa2 )
        return 0;
    
    if( bits[1] < float_to_compare.bits[1] )
        return 1;
    
    if( bits[1] > float_to_compare.bits[1] )
        return 0;
    return 1;
    
}


bool float_128::geq_abs( float_128 & float_to_compare ){
    
    return (!leq_abs(float_to_compare) || eq_abs(float_to_compare) );
}

bool float_128::eq_abs( float_128 & float_to_compare )
{
    if( bits[1] != float_to_compare.bits[1] )
        return 0;
    
    uint64_t first1 = bits[0] | ( 1ULL >> 63 );
    uint64_t first2 = float_to_compare.bits[0] | ( 1ULL >> 63 );
    
    return (first1 == first2);
}


float_128  float_128::add_opposite_signs( float_128 & float_to_add )
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
    
    if( leq_abs(float_to_add ) ){
        // substract mantissa of *this from mantissa of float_to_add
        
        if( exp1 < exp2 ){
            
            shift_bits_in_array_right(mantissa1, exp2-exp1);
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
                
                accumulator = bit2 - bit1;
                
                if( accumulator < 0 ){
                        
                    accumulator += 2;
                    int j = i+1;
                    while( 1 ){
                    
                        if( j < 64 ){
                            if ( ( ( mantissa2[1] >> j ) & 1 ) == 1 ){
                                
                                mantissa2[1] = mantissa2[1] &  ( ~(1 << j ) );
                                
                                break;
                            }
                            else{
                                
                                mantissa2[1] = mantissa2[1] | (1 << j );
                            }
                            
                        }
                        else{
                            
                            if ( ( ( mantissa2[0] >> j ) & 1 ) == 1 ){
                                
                                mantissa2[0] = mantissa2[0] &  ( ~(1 << j ) );
                                
                                break;
                            }
                            else{
                                
                                mantissa2[0] = mantissa2[0] | (1ULL << j );
                            }
                            
                        }
                        
                        j++;
                    }
                    
                }
                
                if( accumulator )
                    result[1] = result[1] | ( 1ULL << i );
            }
            
            
        result[0] = mantissa2[0] - mantissa1[0];
        
        int places_to_shift = 1;
        while( places_to_shift < 50+64 ){
            
            if( places_to_shift < 51 )
                if( (result[0] >> ( 50 - places_to_shift) ) & 1 ){
                    break;
                }
            else
                if( result[1] >> (50 + 64 - places_to_shift) & 1 )
                    break;
                
            places_to_shift++;
            
        }
        
        shift_bits_in_array_left(result, places_to_shift );
            
        
        float_128 resultt;
        // TOD0: shift result to 1 + M, decreasing exp2
        
        
        resultt.bits[0] = result[0];
        resultt.bits[1] = result[1];
    
        resultt.set_exponent(exp2+4096 - places_to_shift);
        if( float_to_add.is_negative() )
            resultt.bits[0] = resultt.bits[0] | (1ULL << 63 );
        
    
        return resultt;
        
    }
    
    float_128 resultt;
    return resultt;
}



