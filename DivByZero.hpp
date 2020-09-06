#ifndef __DIV_BY_ZERO
# define __DIV_BY_ZERO

#include <iostream>

class DivByZero : public std::exception {
    public:
        DivByZero(std::string error);
        ~DivByZero() throw();
        DivByZero    &operator=(DivByZero const &rhs);
        DivByZero(DivByZero const &src);
        virtual const char* what() const throw();

        std::string getError() const;

    private:
        DivByZero();
        std::string _error;
};

#endif