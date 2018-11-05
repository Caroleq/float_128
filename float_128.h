#ifndef FLOAT_128_H
#define FLOAT_128_H

#include <stdlib.h>
// x = (-1)^s*2^(bias-E)*(1+M)



class float_128
{
    
  long long lower_bits;
  long long higher_bits;   // TODO - reimplement to 2-elements array
  
  uint64_t bits[2];
  
public:
    float_128():lower_bits(0), higher_bits(0){}
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
            bits[0] = bits[0] & ( ~( 1 << index ) );
            return;
        }
        
        bits[1] = bits[1] & ( ~( 1 << index ) );   
        
    }
    
    int get_exponent()
    {
            int exponent = 0;
            
            for( int i=126; i>113; i--){
                
             //   std::cout << "bit: " << i << "  value: " <<  (  ( bits[0] >> (i-64) ) & 1 ) << std::endl;
                    
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
    
    
    void only_mantissa_bits( uint64_t mantissa [] )
    {
     
        mantissa[0] = mantissa[1] = 0;
        
        for( int i=0; i<64; i++){
                if( get_bit(i) )
                    mantissa[0] = mantissa[0] | ( 1ULL << i );
        }
        
        for( int i=64; i<114; i++){
                if( get_bit(i) )
                    mantissa[1] = mantissa[1] | ( 1ULL << (i-64) );
                
        }
        
    }
    
    
    
    void add_two_uint64( uint64_t mantissa1 [], uint64_t mantissa2 [], uint64_t result [], int & is_one )
    {
        
        uint8_t accumulator = 0;
        
        result[0] = result[1] = 0;
        
        for( int i=0; i<64; i++){
         
            bool bit1 = ( mantissa1[1] >> i ) & 1; 
            bool bit2 = ( mantissa2[1] >> i ) & 1; 
            
         //   std::cout << "bit1=" << bit1 << " bit2="<< bit2 <<std::endl;
            
            accumulator += bit1 + bit2;
            
            if( (accumulator%2) == 1 )
                result[1] = result[1] | ( 1ULL << i );
            
            if( accumulator > 1)
                accumulator -= 2;
        }
        
        
        
        for( int i=64; i< 113; i++){
         
            bool bit1 = ( mantissa1[0] >> (i-64) ) & 1; 
            bool bit2 = ( mantissa2[0] >> (i-64) ) & 1; 
            
            accumulator += bit1 + bit2;
            

            if( (accumulator%2) == 1 )
                result[0] = result[0] | ( 1ULL << (i-64) );
            
            if( accumulator > 1)
                accumulator -= 2;
        }
        

        
        if( accumulator ){
            is_one = 1;
        }
        else
            is_one = 0;
        
    }
    
    float_128 operator+ ( float_128 & float_to_add ){
        

        
        int exp1 = get_exponent();
        int exp2 = float_to_add.get_exponent();
        
        
        uint64_t mantissa1[2];
        uint64_t mantissa2[2];
        
        
        mantissa1[0] = bits[0]; mantissa1[1] = bits[1];
        mantissa2[0] = float_to_add.bits[0]; mantissa2[1] = float_to_add.bits[1];
        
        if( exp1 > exp2 ){
            mantissa2[0] = mantissa2[0] >> (exp1-exp2);
            mantissa2[1] = mantissa2[1] >> (exp1-exp2);
            mantissa2[0] = mantissa2[0] | ( 1ULL << (64-(exp1-exp2)-15) ); 
        }
        else if( exp1 < exp2 ){
            mantissa1[0] = mantissa1[0] >> (exp2-exp1);
            mantissa1[1] = mantissa1[1] >> (exp2-exp1);
        }
        
        
        
        uint64_t mantissa3[2];
        int is_one;
        
        add_two_uint64( mantissa1, mantissa2, mantissa3, is_one );
        
        int exp = exp1 > exp2? exp1 : exp2;
        
        
        
        if( is_one )
            exp++;
        

        float_128 result;
        
        result.bits[0] = mantissa3[0];
        result.bits[1] = mantissa3[1];
        
        result.set_exponent(exp+4095);
        
        if( get_bit(127) )
            result.set_bit(127);
        
        return result;
        
    }
};

#endif
