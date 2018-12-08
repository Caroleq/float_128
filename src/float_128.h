#ifndef FLOAT_128_H
#define FLOAT_128_H

#include <stdlib.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <math.h>
// x = (-1)^s*2^(bias-E)*(1+M)

/*
 * 
 * Class implementing floating point number of size 128 bits (number stored in two-element uint64_t array
 * It enables arithmetic operations on numbers. May be useful e.g. while summing floating point numbers
 * (greater accuracy then summng `double` or `float` type numbers).
 */


class float_128
{

    /*
     * array storing number information
     * Higher bits are stored in bits[0], lower in bits[1]
     * Bits are index in descending order e.g. 127 bit of number refers to ( bits[0] >> 63 ) & 1 bit
     */
  
  uint64_t bits[2];             // 
  
public:
    float_128(){
        bits[0] = 0;
        bits[1] = 0;
    }
    float_128( int number ){
        /*
         * Creating number from integer
         */
        
        bits[0] = bits[1] = 0;
        
        if( number < 0 )
            set_bit( 127 );

        if( number == 0)
            return;
        
        double to_convert = abs( number );
          
        int exponent = get_expoonent_from_int(abs( number) );
        

        set_exponent( exponent + 4095 );
        set_mantissa(  to_convert );
    }
    
    
    float_128( double number );
    
    float_128( const float_128 & number );
    
    
    uint8_t get_expoonent_from_int( int number){
        /*
         * Function returns exp form
         * number = (-1)^{sign} * 2^(exp) * ( 1 + M )
         * and 0 < M < 1
         */
     

       uint8_t *ptr_to_int = (uint8_t *)&number;
       uint8_t exp = 0;
       
       while(  ( number / (double)(1ULL<<(exp+1)) ) >= 1)
            exp++;
      
        return exp;
        
    }
    
    
    
    void set_mantissa( double number){
        /*
         * Sets mantissa of *this to mantissa stored in `number` 
         */
     
       uint8_t *ptr_to_double = (uint8_t *)&number;

       for( int byte = 0; byte < sizeof(double); byte++)
        {
            uint8_t value = ptr_to_double[byte];
            uint8_t bit;
            
            for(bit = 0; bit < 8; bit++)
            {
                if( value & 1) 
                    set_bit(byte*8+62+bit);
                if( byte * 8 + bit == 51 )
                    return;
                value >>= 1;
            }
        }
    }
    
    
    
    void set_exponent( int exp ){
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

    
    bool get_bit( int index ){
        /* returns `index` bit of stored number*/
     
        if( index < 0 || index > 127 )
            return -1;
        
        if( index > 63 ){
            index -= 64;
            return ( bits[0] >> index ) & 1;
        }
        return ( bits[1] >> index ) & 1;
    }
    
    
    void set_bit( int index ){
        /*  Sets bit `index` of `bits` to 1 */
     
        if( index < 0 || index > 127 )
            return;
        
        if( index > 63 ){
            index -= 64;
            bits[0] = bits[0] | ( 1ULL << index );

            return;
        }
        
        bits[1] = bits[1] | ( 1ULL << index );
    }
    
    
    void clear_bit( int index ){
        /*  Sets bit `index` of `bits` to 0 */
     
        if( index < 0 || index > 127 )
            return;
        
        if( index > 63 ){
            index -= 64;
            bits[0] = bits[0] & ( ~( 1ULL << index ) );
            return;
        }
        
        bits[1] = bits[1] & ( ~( 1ULL << index ) );   
        
    }
    
    int get_exponent()
    {
        /*
         * Returns exponent of number stored in bits
         */
            int exponent = 0;
            
            for( int i=126; i>113; i--){
                exponent = exponent*2 + (  ( bits[0] >> (i-64) ) & 1 ); 
                
            }
            
            return exponent - 4096;
    }
    
    
    const std::string binary_representation(){
        /*
         * Returns string representing stored number from the most significent
         * bit to the least significante one. Sign, exponent and mantissa bits
         * are saparated with space.
         */
        
        std::string binary = "";
     
        for( int i=127; i>-1 ; i--){
            binary += ( get_bit(i)+48 );  
            if( i == 127 || i == 114 )
                binary += " ";
        }
     
        return binary;
    }
    
    

    
    bool is_negative(){
        return ( bits[0] >> 63 ) & 1;
    }
    
    float_128 operator+ ( float_128 & float_to_add ){
      
        if( is_negative() && float_to_add.is_negative() ||  !is_negative() && !float_to_add.is_negative() ){
            return add_same_sign(float_to_add);
        }
        
        return add_opposite_signs( float_to_add );
        
    }
    
    float_128 operator- ( float_128 & float_to_substract  ){
     
        if( is_negative() && float_to_substract.is_negative() ||  !is_negative() && !float_to_substract.is_negative() ){
            return add_opposite_signs( float_to_substract );
        }
        

        return add_same_sign( float_to_substract );
        
    }
    
    float_128 operator* (  float_128 & float_to_multiply  );
    
    float_128 add_same_sign( float_128 & float_to_add);
    float_128 add_opposite_signs( float_128 & float_to_add );

    bool leq_abs(  float_128 & float_to_compare );
    bool geq_abs( float_128 & float_to_compare );
    bool eq_abs(  float_128 & float_to_compare );
    
    bool operator<= ( float_128 & float_to_compare){
        
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
    
    bool operator== (  float_128 & float_to_compare){
      return ( float_to_compare.bits[0] == bits[0] || float_to_compare.bits[1] == bits[1] );   
    }
    
    bool operator>=( float_128 & float_to_compare ){
        
        return ( !(*this<=float_to_compare) || *this==float_to_compare );
    }
    
};

#endif
