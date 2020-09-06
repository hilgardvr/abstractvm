#include "OverUnderFlow.hpp"

OverUnderFlow::OverUnderFlow() {};

OverUnderFlow::OverUnderFlow(std::string error) : _error(error) {};

OverUnderFlow::~OverUnderFlow() throw() {}
OverUnderFlow    &OverUnderFlow::operator=(OverUnderFlow const &rhs) {
    this->_error = rhs.getError();
    return *this;
}
OverUnderFlow::OverUnderFlow(OverUnderFlow const &src) {
    operator=(src);
}

const char* OverUnderFlow::what() const throw() {
    return "Operation results in over/underflow";
}

std::string     OverUnderFlow::getError() const {
    return this->_error;
}