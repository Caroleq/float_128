#include "float_128.h"

extern void display_array( uint64_t array[] );
extern void shift_bits_in_array_left( uint64_t array[] , int positions );
extern void shift_bits_in_array_right( uint64_t array [], int shift );
extern void set_array( uint64_t bits1[], int arr1[], bool set_1, uint64_t bits2[], int arr2[], bool set_2);
extern int convert_to_mantissa( uint64_t mantissa[], int arr[] );
extern void multiply_mantissas( uint64_t mantissa1[], uint64_t mantissa2[], int result [] );


/* <---------------------------------------------------          PRIVATE METHODS        ---------------------------------------------------> */

float_128 float_128::add_same_sign ( const float_128 & float_to_add ) const
{
    if( bits[0] == 0 && bits[1] == 0 )
        return float_to_add;
    if( float_to_add.bits[0] ==0 && float_to_add.bits[1] == 0 )
        return *this;
    
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
        if( 50+exp2-exp1 > -1 )
            mantissa2[0] = mantissa2[0] | ( 1ULL << (50+exp2-exp1) );
        else{
            mantissa2[1] = mantissa2[1] | ( 1ULL << (114+exp2-exp1) );
        }
    }
    else if( exp1 < exp2 ){
        exp = exp2;
        one1 = 0;
        shift_bits_in_array_right(mantissa1, exp2-exp1);
        if( 50-exp2+exp1 > -1 )
            mantissa1[0] = mantissa1[0] | ( 1ULL << (50-exp2+exp1) );
        else
            mantissa1[1] = mantissa1[1] | ( 1ULL << (114-exp2+exp1) );
    }
    else{
        exp = exp1 + 1;
        shift_bits_in_array_right(mantissa1, 1);
        shift_bits_in_array_right(mantissa2, 1);
        
        mantissa1[0] = mantissa1[0] | ( 1ULL << 49 );
        mantissa2[0] = mantissa2[0] | ( 1ULL << 49 );
        
        mantissa1[0] &= ~(1ULL << 50 );
        mantissa2[0] &= ~(1ULL << 50 );
    }

    int accumulator = 0;
    
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

        if( (accumulator%2) == 1 )
            result[0] = result[0] | ( 1ULL << i );
            
        if( accumulator > 1)
            accumulator = accumulator >> 1;
        else
            accumulator = 0;
    }
        
    if( accumulator + one1 + one2  == 2){
        exp++;
        shift_bits_in_array_right(result, 1);
    }
    
    if( is_negative() )
        result[0] = result[0] | (1ULL <<63);
    
    float_128 resultt;

    resultt.bits[0] = result[0];
    resultt.bits[1] = result[1];
    
    resultt.set_exponent(exp);
    
    return resultt;
}



float_128  float_128::add_opposite_signs( const float_128 & float_to_add ) const
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

    result[0] = 0;
    result[1] = 0;
        
    int tmp1[50+64+1];
    int tmp2[50+64+1];
        
    int tmp[50+64+1];
    
    if( leq_abs(float_to_add ) ){
        
        if( exp1 < exp2 ){
            shift_bits_in_array_right(mantissa1, exp2-exp1);
            if( 50 - exp2 + exp1 > -1 )
                mantissa1[0] = mantissa1[0] | ( 1ULL << ( 50 - exp2 + exp1 ) );
            else
                mantissa1[1] = mantissa1[1] | ( 1ULL << ( 114 - exp2 + exp1 ) );
             set_array( mantissa1, tmp1, 0, mantissa2, tmp2, 1);
        }
        else{
            
             set_array( mantissa1, tmp1, 0, mantissa2, tmp2, 0);
        }
    
        for( int i=114; i>-1; i--){
            
            tmp[i] = tmp2[i] - tmp1[i];
                
            if( tmp[i] < 0 ){
                tmp[i] += 2;
                tmp2[i-1]--;
            }
        }
        
        int places_to_shift = convert_to_mantissa(result, tmp );

        float_128 resultt;
        
        if( places_to_shift == -1 )
            return resultt;
        
        resultt.bits[0] = result[0];
        resultt.bits[1] = result[1];
        
        if( exp2 == exp1 && places_to_shift )
            places_to_shift--;
    
        resultt.set_exponent(exp2 - places_to_shift);
        if( float_to_add.is_negative() )
            resultt.set_bit( 127 );
    
        return resultt;
        
    }
    
    if( exp2 < exp1 ){
        shift_bits_in_array_right(mantissa2, exp1-exp2);
        if(  50 - exp1 + exp2 > -1) 
            mantissa2[0] = mantissa2[0] | ( 1ULL << ( 50 - exp1 + exp2) );
        else
            mantissa2[1] = mantissa2[1] | ( 1ULL << ( 114 - exp1 + exp2) );
        
        set_array( mantissa1, tmp1, 1, mantissa2, tmp2, 0);
    }
    else
        set_array( mantissa1, tmp1, 0, mantissa2, tmp2, 0);
    
    for( int i=114; i>-1; i--){
            
       tmp[i] = tmp1[i] - tmp2[i];
                
        if( tmp[i] < 0 ){
            tmp[i] += 2;
            tmp1[i-1]--;
        }
    }
    
    int places_to_shift = convert_to_mantissa(result, tmp );
    if( exp2 == exp1 && places_to_shift )
            places_to_shift--;
               
    float_128 resultt;
    
    resultt.bits[0] = result[0];
    resultt.bits[1] = result[1];
    
    resultt.set_exponent(exp1 - places_to_shift);
    
    if( is_negative() )
        resultt.set_bit(127);
    
    return resultt;
}


