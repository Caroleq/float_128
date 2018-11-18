#ifndef FLOAT_128_H
#define FLOAT_128_H

#include <stdlib.h>
#include <string>
#include <cstdint>
#include <iostream>
// x = (-1)^s*2^(bias-E)*(1+M)



class float_128
{
    
  //long long lower_bits;
 // long long higher_bits;   // TODO - reimplement to 2-elements array
  
  uint64_t bits[2];
  
public:
    float_128(){
        bits[0] = 0;
        bits[1] = 0;
    }
    float_128( int number ){
        
        bits[0] = bits[1] = 0;
        
        if( number < 0 )
            set_bit(127);

        if( number == 0)
            return;
        
        double to_convert = abs( number );
          
        int exponent = get_expoonent_from_int(abs( number) );
        set_exponent( exponent + 4095 );
        set_mantissa(  to_convert );
    }
    
    
    float_128( double number ) {

        bits[0] = bits[1] = 0;
        
        if( number < 0 )
            set_bit(127);

        if( number == 0)
            return;
        
        double to_convert = abs( number );
        
        int exponent = get_exponent_from_double( to_convert );
        set_exponent( exponent + 4095 );
        set_mantissa( to_convert );
        
        
    }
    
    
    double get_pow_float( int power ){
        
        double result = 1;
        
        while( power > 0 ){
            result /= (double)2.0;
            power--;
        }

        return result;
    }
    
    int get_expoonent_from_int( int number){
     

       uint8_t *ptr_to_int = (uint8_t *)&number;
       int  sum = 0;

       for( int byte = sizeof(int)-1; byte > -1; byte--)
        {
            int value = ptr_to_int[byte];
            int bit;
            
            for(bit = 0; bit < 8; bit++ )
            {
                if( sum > 0 &&  ( (double)number )/( ( sum * 2 ) + ( (value>>(7-bit) ) & 1) )<= 1)
                    return sum;
                
                sum = ( sum * 2 ) + ( (value>>(7-bit) ) & 1);
                
            }
        }
        
        return sum;
        
    }
    
    
    
    void set_mantissa( double number){
     

       uint8_t *ptr_to_double = (uint8_t *)&number;

       for( int byte = 0; byte < sizeof(double); byte++)
        {
            uint8_t value = ptr_to_double[byte];
            int bit;
            
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
    


    int get_exponent_from_double( double number )
    {
        
        uint8_t *ptr_to_double = (uint8_t *)&number;
        int  sum = 0;
        for( int byte = sizeof(double)-1; byte > 5; byte--)
        {
            int value = ptr_to_double[byte];
            int bit;
            
            for(bit = 0; bit < 8; bit++ )
            {
                
                sum = ( sum * 2 ) + ( (value>>(7-bit) ) & 1);
                if( byte * 8 + bit == 52 )
                    return sum - 2047;
            }
            
            if( byte * 8 + bit == 52 )
                    break;

        }
        return sum - 2047;
        
    }
    
    
    void set_exponent( int exp ){
        
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
     
        if( index < 0 || index > 127 )
            return -1;
        
        if( index > 63 ){
            index -= 64;
            return ( bits[0] >> index ) & 1;
        }
        return ( bits[1] >> index ) & 1;
    }
    
    
    void set_bit( int index ){
        
     
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
            int exponent = 0;
            
            for( int i=126; i>113; i--){
                exponent = exponent*2 + (  ( bits[0] >> (i-64) ) & 1 ); 
                
            }
            
            return exponent - 4096;
        
        
    }
    
    
    const std::string binary_representation(){
        
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
            float_128 result = add_absolute_values(float_to_add);
            return result;
        }
        
        float_128 result;
        
        return result;
        
    }
    
    float_128 add_absolute_values( float_128 & float_to_add);
    void shift_bits_in_array( uint64_t array [], int shift );
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
