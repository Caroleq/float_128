#include "float_128.h"

extern void display_array( uint64_t array[] );
extern void shift_bits_in_array_left( uint64_t array[] , int positions );
extern void shift_bits_in_array_right( uint64_t array [], int shift );
extern void set_array( uint64_t bits1[], int arr1[], bool set_1, uint64_t bits2[], int arr2[], bool set_2);
extern int convert_to_mantissa( uint64_t mantissa[], int arr[] );
extern void multiply_mantissas( uint64_t mantissa1[], uint64_t mantissa2[], int result [] );





float_128::float_128 ( std::string binary ){
    /*
     * Creates a number from string of length 128 consisitng of ones andzeros
     * 
     */
    
    
    bits[0] = bits[1] = 0;
    
    if( binary.size() != 128 ){
        return;
    }
    
    for( int i=0; i<128; i++){
        
         if( binary[i] == 48 ){
            continue;
         }
         else if ( binary[i] == 49 ){
            set_bit( i );
         }
         else{
            bits[0] = bits[1] = 0;
            return;
         }
    }
    
}

float_128::float_128( double number ) {
    /*
     * Creating number from double
     */
    bits[0] = bits[1] = 0;
        
    if( number < 0 )
        set_bit(127);

    if( number == 0)
        return;
        
    double to_convert = abs( number );
        
    int exponent;
    frexp (number , &exponent);
        
    set_exponent( exponent - 1 + 4095 );
    set_mantissa( to_convert );
        
}


float_128::float_128( const float_128 & number )
{
    
    //std::cout << "Konstruktor kopiujacjy" << std::endl;
    
    bits[0] = number.bits[0];
    bits[1] = number.bits[1];
    
}

float_128 float_128::add_same_sign( float_128 & float_to_add )
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
  //  std::cout << "exp1=" << exp1 << " exp2=" << exp2 << std::endl;
    
    if( exp1 > exp2 ){
        exp = exp1;
        one2 = 0;
        shift_bits_in_array_right(mantissa2, exp1-exp2);
      //  mantissa2[0] = mantissa2[0] | ( 1ULL << (50+exp2-exp1) );
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
        else{
            mantissa1[1] = mantissa1[1] | ( 1ULL << (114-exp2+exp1) );
        }
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

 /*   std::cout << "matisissa1: " << std::endl;
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
    
    resultt.set_exponent(exp+4095);
    
  
    
    return resultt;
    
}




bool float_128::leq_abs( float_128 & float_to_compare ){
  
    int exp1 = get_exponent();
    int exp2 = float_to_compare.get_exponent();
    
    if( exp1 < exp2 )
        return 1;
    if( exp1 > exp2)
        return 0;
    
    uint64_t mantissa1 = ( ( (bits[0] << 1) >> 1 )  >> 13 ) << 13;
    uint64_t mantissa2 = ( (( float_to_compare.bits[0] << 1) >> 1 )  >> 13 ) << 13;
    

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
        
        //display_array(result);
        resultt.bits[0] = result[0];
        resultt.bits[1] = result[1];
        
        if( exp2 == exp1 && places_to_shift )
            places_to_shift--;
    
        resultt.set_exponent(exp2+4095 - places_to_shift);
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
    else{
        set_array( mantissa1, tmp1, 0, mantissa2, tmp2, 0);
    }
    
    
    

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
    
    resultt.set_exponent(exp1+4095 - places_to_shift);
    
    if( is_negative() )
        resultt.set_bit(127);
    
    return resultt;
}


float_128 float_128::operator* (  float_128 & float_to_multiply  )
{
    float_128 result;
    
    uint64_t mantissa1[2];
    uint64_t mantissa2[2];
    
    mantissa1[0] = bits[0]; mantissa1[1] = bits[1];
    mantissa2[0] = float_to_multiply.bits[0]; mantissa2[1] = float_to_multiply.bits[1];
    
    int result_mantissa[230];
    
    multiply_mantissas( mantissa1, mantissa2, result_mantissa );
    
    int exp = 0;
    
    int start = 2;
    if( result_mantissa[0]  ){
        start = 1;
        exp += 1;
    }   
    
    exp += get_exponent() + float_to_multiply.get_exponent();
    
    for( int i=0; i < 114; i++ )
        if( result_mantissa[ i + start ] ){
            result.set_bit(  113 - i);
        }
    
        
    result.set_exponent( exp + 4095 );
    
    if( is_negative() && !float_to_multiply.is_negative() ||  !is_negative() && float_to_multiply.is_negative())
        result.set_bit( 127 );
    
        
    return result;
}



bool float_128::operator<= ( float_128 & float_to_compare){
        
        if( is_negative() && !float_to_compare.is_negative() ){
                return 1;
        }
        if( !is_negative() && float_to_compare.is_negative() ){
                return 0;
        }
        
        int sign = ( bits[0] >> 63 ) & 1;
        
        switch( sign ){
            
            case 0:
                return leq_abs( float_to_compare  );
                break;
            case 1:
                return geq_abs( float_to_compare );
        }
        
        return 0;
    }

    
float_128 & float_128::operator += ( float_128 & float_to_add )
{
    float_128 tmp = *this + float_to_add;
    bits[0] = tmp.bits[0];
    bits[1] = tmp.bits[1];
    
    return *this;
}


float_128 & float_128::operator -= ( float_128 & float_to_add )
{
    float_128 tmp = *this - float_to_add;
    bits[0] = tmp.bits[0];
    bits[1] = tmp.bits[1];
    
    return *this;
}

float_128 float_128::operator= (  float_128 & float_to_assign)
{
        if( &float_to_assign == this)
            return *this;
        
        bits[0] = float_to_assign.bits[0];
        bits[1] = float_to_assign.bits[1];
        
        return *this;
}


