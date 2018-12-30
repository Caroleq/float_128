#ifndef FLOAT_128_H
#define FLOAT_128_H

#include <stdlib.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <math.h>

#include "exceptions/invalid_index.h"
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
       `bits` is an array storing number information
       Higher bits are stored in bits[0], lower in bits[1]
       Bits are indiced in descending order e.g. 127 bit of number refers to ( bits[0] >> 63 ) & 1 bit
     */
  
  uint64_t bits[2];             
  
public:
    
    /* Constructors */
    
    float_128 ();
    
    float_128 ( int number );
    
    float_128 ( std::string binary );
    
    float_128( double number );
    
    float_128( const float_128 & number );
    
    /* Getters */
    
    uint8_t get_exponent_from_int( int number ) const;
    
    bool get_bit( int index ) const;
    
    int get_exponent() const;
    
    const std::string binary_representation () const;
    
    bool is_negative() const;
    
    /* Setters */
    
    void set_mantissa( double number );
    
    void set_exponent( int exp );
    
    void set_bit( int index );
    
    void clear_bit( int index );
    
    /* Operators */
    
    float_128 operator= ( const float_128 & float_to_assign );
    
    float_128 operator+ ( const float_128 & float_to_add ) const;
    
    float_128 operator- ( const float_128 & float_to_substract ) const;
    
    float_128 operator* ( const float_128 & float_to_multiply ) const;
    
    float_128 & operator+= ( const float_128 & float_to_add );
    
    float_128 & operator-= ( const float_128 & float_to_add );
    
    bool operator<= ( const float_128 & float_to_compare) const;
    
    bool operator>= ( const float_128 & float_to_compare ) const;
    
    bool operator== ( const float_128 & float_to_compare) const;
    
    /* Additional comparison methods */
    
    bool leq_abs ( const float_128 & float_to_compare ) const;
    
    bool geq_abs ( const float_128 & float_to_compare ) const;
    
    bool eq_abs ( const float_128 & float_to_compare ) const;
    
    
    /* Private methods used by class oparators */
    
private:
    float_128 add_same_sign ( const float_128 & float_to_add ) const;
    
    float_128  add_opposite_signs ( const float_128 & float_to_add ) const;
    
};

#endif
