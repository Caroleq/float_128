#ifndef FLOAT_128_H
#define FLOAT_128_H

#include <stdlib.h>
// x = (-1)^s*2^(bias-E)*(1+M)



class float_128
{
    
  long long lower_bits;
  long long higher_bits;   // TODO - reimplement to 2-elements array
  
public:
    float_128():lower_bits(0), higher_bits(0){}
    float_128( int number ){
        lower_bits = 0;
        higher_bits = 0;
        
        if( number < 0 )
            set_bit(127);

        if( number == 0)
            return;
        
        double to_convert = abs( number );
        
        int exponent = 4095;
        while( to_convert > 1){
            exponent += 1;
            if(  to_convert / 2 < 1 )
                break;
            to_convert /= 2;
           
        }
          
        set_exponent( exponent );
        set_mantissa_from_double( ( double )number);
    }
    
    
    float_128( double number ) {
        lower_bits = 0;
        higher_bits = 0;
        
        if( number < 0 )
            set_bit(127);

        if( number == 0)
            return;
        
        double to_convert = abs( number );
        
        int exponent=4095;
        while( to_convert > 1){
            exponent += 1;
            if(  to_convert / 2 < 1 )
                break;
            to_convert /= (double)2;
           
        }
        set_exponent_from_double( number );
        set_mantissa_from_double( number );
        
       // std::cout << "exponent: " << exponent << " mantissa: " << to_convert << std::endl;
        

        // std::cout <<  std::endl;
        
    }
    
    
    double get_pow_float( int power ){
        
        double result = 1;
        
        while( power > 0 ){
            result /= (double)2.0;
            power--;
        }
        //std::cout << "power=" << power << std::endl;
        
        return result;
    }
    
    
    void set_mantissa_from_double( double number){
     

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
    
    
    void set_exponent_from_double( double number )
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
                    break;
            }
            
            if( byte * 8 + bit == 52 )
                    break;

        }
        
        
        sum = sum - 2047 + 4095;
        set_exponent( sum );
        
        
        
        
    }
    
    
    void set_exponent( int exp ){
        
      //  std::cout << exp << std::endl;
        
        for( int i=114; i<127; i++){
            
            bool bit = exp & 1;
          //  std::cout << exp << " " << (exp & 1 ) << std::endl;
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
            return ( higher_bits >> index ) & 1;
        }
        

        return ( lower_bits >> index ) & 1;
    }
    
    
    void set_bit( int index ){
        
     //   std::cout << "setting " << index << std::endl;
     
        if( index < 0 || index > 127 )
            return;
        
        if( index > 63 ){
            index -= 64;
//            std::cout << binary_representation() << std::endl;
            higher_bits = higher_bits | ( 1ULL << index );// + ( (higher_bits << (63 - index) ) >> (63-index) ) ;
  //          std::cout <<"set: " << get_bit(index+64) << "\n";
       //     std::cout << binary_representation() << std::endl;
            return;
        }
        
        lower_bits = lower_bits | ( 1ULL << index );
    }
    
    
    void clear_bit( int index ){
     
        if( index < 0 || index > 127 )
            return;
        
        if( index > 63 ){
            index -= 64;
            higher_bits = higher_bits & ( ~( 1 << index ) );
            return;
        }
        
        lower_bits = lower_bits & ( ~( 1 << index ) );   
        
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
    
    float_128 operator+ ( const float_128 & float_to_add ){
        
        
        return float_128();
        
    }
};

#endif
