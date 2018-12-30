#ifndef BINARY_REPRESENTATION_ERROR_H
#define BINARY_REPRESENTATION_ERROR_H

#include <exception>

class binary_representation_error: public std::exception
{
    
    std::string message;
public:
    binary_representation_error( std::string msg = "Invalid binary representation" ):message(msg){}
    
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
    
    friend std::ostream& operator<<( std::ostream& os, binary_representation_error const error )
    {
        os << error.message;
        return os;
    }
    
};


#endif
