#include "float_128.h"

extern void display_array( uint64_t array[] );
extern void shift_bits_in_array_left( uint64_t array[] , int positions );
extern void shift_bits_in_array_right( uint64_t array [], int shift );
extern void set_array( uint64_t bits1[], int arr1[], bool set_1, uint64_t bits2[], int arr2[], bool set_2);
extern int convert_to_mantissa( uint64_t mantissa[], int arr[] );
extern void multiply_mantissas( uint64_t mantissa1[], uint64_t mantissa2[], int result [] );


/* <---------------------------------------------------          CONSTRUCTORS        ---------------------------------------------------> */

float_128::float_128 ()
{
    
    bits[0] = 0;
    bits[1] = 0;
}

float_128::float_128 ( int number )
{
    /* Creating number from integer */
        
    bits[0] = bits[1] = 0;
        
    if( number < 0 )
        set_bit( 127 );

    if( number == 0)
        return;
        
    double to_convert = abs( number );
          
    int exponent = get_exponent_from_int(abs( number) );
    set_exponent( exponent + 4095 );
    set_mantissa(  to_convert );
}



float_128::float_128 ( std::string binary ) 
{
    /* Creates a number from string of length 128 consisitng of ones and zeros */ 
    
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

float_128::float_128( double number ) 
{
    /*
      Creating number from double
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
    
    bits[0] = number.bits[0];
    bits[1] = number.bits[1];
    
}


/* <---------------------------------------------------          GETTERS       ---------------------------------------------------> */

uint8_t float_128::get_exponent_from_int( int number ) const
{
    /*
      Function returns exp from
      number = (-1)^{sign} * 2^(exp) * ( 1 + M )
      where 0 < M < 1
    */    
     
    uint8_t exp = 0;
       
    while(  ( number / (double)( 1ULL<<(exp+1) ) ) >= 1 )
        exp++;
      
    return exp;
        
}

bool float_128::get_bit( int index ) const
{
    /* returns `index` bit of stored number*/
     
    if( index < 0 || index > 127 )
        return -1;
        
    if( index > 63 ){
        index -= 64;
        return ( bits[0] >> index ) & 1;
    }
        
    return ( bits[1] >> index ) & 1;
}


int float_128::get_exponent() const
{
    /*
        Returns exponent of number stored in bits
    */
    int exponent = 0;
            
    for( int i=126; i>113; i--)
        exponent = exponent*2 + (  ( bits[0] >> (i-64) ) & 1 ); 
           
            
    return exponent - 4095;
}

bool float_128::is_negative() const 
{
    return ( bits[0] >> 63 ) & 1;
}

const std::string float_128::binary_representation() const
{
    /*
        Returns string representing stored number from the most significent
        bit to the least significante one. Sign, exponent and mantissa bits
        are saparated with space.
    */
        
    std::string binary = "";
     
    for( int i=127; i>-1 ; i--){
        binary += ( get_bit(i)+48 );  
        if( i == 127 || i == 114 )
            binary += " ";
    }
     
    return binary;
}

/* <---------------------------------------------------          SETTERS        ---------------------------------------------------> */

void float_128::set_mantissa( double number )
{
        /*
         * Sets mantissa of *this to mantissa stored in `number` 
         */
     
    uint8_t *ptr_to_double = (uint8_t *)&number;

    for( int byte = 0; byte < (uint8_t)sizeof(double); byte++)
    {
        uint8_t value = ptr_to_double[byte];
        uint8_t bit;
            
        for(bit = 0; bit < 8; bit++)
        {
            if( value & 1 ) 
                set_bit( byte*8 + 62 + bit );
            if( byte*8 + bit == 51 )
                return;
            value >>= 1;
        }
    }
}


void float_128::set_exponent( int exp )
{
        /*  Sets exponent to `exp` */
        
        
        for( int i=114; i<127; i++){
            
            bool bit = exp & 1;
            if( bit )
                set_bit(i);
            else
                clear_bit(i);
            exp = exp >> 1;
        }
        
}


void float_128::set_bit( int index ) 
{
    /*  Sets bit `index` of `bits` to 1 */
     
    if( index < 0 || index > 127 )
        throw invalid_index_error();
        
    if( index > 63 ){
        index -= 64;
        bits[0] = bits[0] | ( 1ULL << index );

        return;
    }
        
    bits[1] = bits[1] | ( 1ULL << index );
}


void float_128::clear_bit( int index )
{
    /*  Sets bit `index` of `bits` to 0 */
     
    if( index < 0 || index > 127 )
        throw invalid_index_error();
        
    if( index > 63 ){
        index -= 64;
        bits[0] = bits[0] & ( ~( 1ULL << index ) );
        return;
    }
        
    bits[1] = bits[1] & ( ~( 1ULL << index ) );   
}

  
