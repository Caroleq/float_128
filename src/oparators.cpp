#include "float_128.h"

extern void display_array( uint64_t array[] );
extern void shift_bits_in_array_left( uint64_t array[] , int positions );
extern void shift_bits_in_array_right( uint64_t array [], int shift );
extern void set_array( uint64_t bits1[], int arr1[], bool set_1, uint64_t bits2[], int arr2[], bool set_2);
extern int convert_to_mantissa( uint64_t mantissa[], int arr[] );
extern void multiply_mantissas( uint64_t mantissa1[], uint64_t mantissa2[], int result [] );



/* <---------------------------------------------------          OPERATORS        ---------------------------------------------------> */



float_128 float_128::operator= ( const float_128 & float_to_assign )
{
    if( &float_to_assign == this)
        return *this;
        
    bits[0] = float_to_assign.bits[0];
    bits[1] = float_to_assign.bits[1];
        
    return *this;
}

float_128 float_128::operator+ ( const float_128 & float_to_add ) const 
{
      
    if( ( is_negative() && float_to_add.is_negative() ) || ( !is_negative() && !float_to_add.is_negative() ) ) {
        return add_same_sign(float_to_add);
    }
        
    return add_opposite_signs( float_to_add );
        
}

float_128 float_128::operator- ( const float_128 & float_to_substract  ) const
{
     
    if( ( is_negative() && float_to_substract.is_negative() ) || ( !is_negative() && !float_to_substract.is_negative() ) ){
            
        return add_opposite_signs( float_to_substract );
    }

    return add_same_sign( float_to_substract );
        
}


float_128 float_128::operator* ( const float_128 & float_to_multiply ) const
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
    
    if( ( is_negative() && !float_to_multiply.is_negative() ) || ( !is_negative() && float_to_multiply.is_negative() ) )
        result.set_bit( 127 );
    
        
    return result;
}


float_128 & float_128::operator += ( const float_128 & float_to_add )
{
    float_128 tmp = *this + float_to_add;
    bits[0] = tmp.bits[0];
    bits[1] = tmp.bits[1];
    
    return *this;
}


float_128 & float_128::operator -= ( const float_128 & float_to_add )
{
    float_128 tmp = *this - float_to_add;
    bits[0] = tmp.bits[0];
    bits[1] = tmp.bits[1];
    
    return *this;
}

bool float_128::operator<= ( const float_128 & float_to_compare ) const
{
        
    if( is_negative() && !float_to_compare.is_negative() )
            return 1;
        
    if( !is_negative() && float_to_compare.is_negative() )
            return 0;

        
    int sign = ( bits[0] >> 63 ) & 1;
        
    switch( sign )
    {
            
        case 0:
            return leq_abs( float_to_compare  );
            break;
        case 1:
            return geq_abs( float_to_compare );
    }
        
    return 0;
}



bool float_128::operator>=( const float_128 & float_to_compare ) const 
{
    return ( !(*this<=float_to_compare) || *this==float_to_compare );
}


bool float_128::operator== ( const float_128 & float_to_compare) const 
{
    return ( float_to_compare.bits[0] == bits[0] || float_to_compare.bits[1] == bits[1] );   
}


/* <---------------------------------------------------          ADDITIONAL COMPARISON METHODS        ---------------------------------------------------> */

bool float_128::leq_abs( const float_128 & float_to_compare ) const
{
  
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


bool float_128::geq_abs( const float_128 & float_to_compare ) const
{
    return (!leq_abs(float_to_compare) || eq_abs(float_to_compare) );
}

bool float_128::eq_abs( const float_128 & float_to_compare ) const
{
    if( bits[1] != float_to_compare.bits[1] )
        return 0;
    
    uint64_t first1 = bits[0] | ( 1ULL >> 63 );
    uint64_t first2 = float_to_compare.bits[0] | ( 1ULL >> 63 );
    
    return (first1 == first2);
}


