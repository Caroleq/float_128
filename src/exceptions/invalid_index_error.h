#ifndef INVALID_INDEX_ERROR_H
#define INVALID_INDEX_ERROR_H

#include <exception>

class invalid_index_error: public std::exception
{
    
    std::string message;
public:
    invalid_index_error( std::string msg = "Invalid index error" ):message(msg){}
    
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
    
    friend std::ostream& operator<<( std::ostream& os, invalid_index_error const error ){
        os << error.message;
        return os;
   }
    
};


#endif
