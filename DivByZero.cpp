#include "DivByZero.hpp"

DivByZero::DivByZero() {};

DivByZero::DivByZero(std::string error) : _error(error) {};

DivByZero::~DivByZero() throw() {}
DivByZero    &DivByZero::operator=(DivByZero const &rhs) {
    this->_error = rhs.getError();
    return *this;
}
DivByZero::DivByZero(DivByZero const &src) {
    operator=(src);
}

const char* DivByZero::what() const throw() {
    return "Cannot divide by zero";
}

std::string     DivByZero::getError() const {
    return this->_error;
}