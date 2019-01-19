#include "float_128.h"

void mantissa_to_decimal( std::vector<char> & dec_representation, const float_128 float_to_print );
void add_bit( std::vector<char> & dec_representation, int index );
void divide_two( std::vector<char> & vec );
void multiply_twice( std::vector<char> & vec );

std::ostream& operator<< ( std::ostream & stream, const float_128 & float_to_print )
{
    
    std::vector<char> dec_representation;
    mantissa_to_decimal( dec_representation,  float_to_print );
    
    int iteration_count = float_to_print.get_exponent();
    
    if( iteration_count > 0 )
        for( int i=0; i<iteration_count; i++ )
            multiply_twice( dec_representation );
    else if ( iteration_count < 0 )
        ;
    
    for( int i=0; i< (int)dec_representation.size(); i++ )
        if( dec_representation[i] == '.' ){
            if( i != (int)dec_representation.size()-1 )
                stream << '.';
        }
        else
            stream << char( dec_representation[i] + 48 );
    return stream;
}




void mantissa_to_decimal( std::vector<char> & dec_representation, const float_128 float_to_print )
{
    dec_representation.clear();
    dec_representation.push_back( 1 );
    
    for( int i=113; i>-1; i-- )
    {
        if( float_to_print.get_bit( i ) )
            add_bit( dec_representation, 114-i );
        
    }
    
    
    dec_representation.insert( dec_representation.begin()+1, '.' );
    
}

void add_bit( std::vector<char> & dec_representation, int index )
{
    
    std::vector<char> single;
    single.push_back(1);
    
   // single.push_back(1);
    
    for( int i=0; i<index; i++ )
    {
        divide_two( single );
        
    }
        
    if( dec_representation.size() > single.size() )
        single.resize( dec_representation.size(), 0 );
    else if ( dec_representation.size() < single.size() )
        dec_representation.resize( single.size(), 0 );
    
    for( int i=dec_representation.size()-1; i>-1; i-- )
    {
        dec_representation[i] += single[i];
        if( dec_representation[i] > 9 )
        {
            dec_representation[i] -= 10;
            dec_representation[i-1] += 1;
        }
    }
    
}

void divide_two( std::vector<char> & vec )
{
    
    bool add_five = false;
    
    for( int i=0; i< (int)vec.size(); i++ )
    {
        int remnant = vec[i] % 2;
        vec[i] = vec[i]/2 + 5*add_five;
        add_five = false;
        if( remnant )
            add_five = true;
    }
    
    vec.push_back(5);
    
}

void multiply_twice( std::vector<char> & vec )
{
   int accumulator = 0;   
   for( int i=vec.size()-1; i>-1; i-- )
   {
      if( vec[i] == '.' )
          continue;
      
      vec[i] = vec[i] * 2 + accumulator;   
      if( vec[i] > 9 )
      {
        vec[i] -= 10;
        accumulator = 1;
      }
      else
          accumulator = 0;
   }
   
   if( accumulator )
   {
      vec.insert(vec.begin(), 1 ); 
   }
    
}
