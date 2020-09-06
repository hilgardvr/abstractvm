#ifndef __OPERANDS_HPP
# define __OPERANDS_HPP

#include "IOperand.hpp"
#include "eOperandType.hpp"
#include <iostream>
#include <string>
#include "OperandFactory.hpp"
#include "DivByZero.hpp"
#include "OverUnderFlow.hpp"
#include <math.h>
#include "eMinMax.hpp"
#include <limits>


template <class T>
class Operands : public IOperand {
    public:
        Operands(T i, eOperandType e): _numValue(i), _e(e)  {
            _stringValue = new std::string(std::to_string(i));
            _factory = new OperandFactory();
        }

        ~Operands() {
            //std::cout << "deleting memory for: " << *_stringValue << "\n";
            delete this->_stringValue;
            if (this->_factory)
                delete this->_factory;
        }

        Operands    &operator=(Operands const &rhs) {
            this->_stringValue = new std::string(rhs.toString());
            this->_numValue = (T)( std::stod(rhs.toString()));
            return *this;
        }

        Operands(Operands const &src){
            this->operator=(src);
        }

        virtual int				getPrecision(void) const{
            return _e;
        }

        virtual eOperandType	getType(void) const{
            return _e;
        }

        virtual IOperand const	*operator+( IOperand const & rhs ) const {
            double a = stod(this->toString());
            double b = stod(rhs.toString());
            eOperandType type = this->getType();
            if (rhs.getType() > type) {
                type = rhs.getType();
            }
            if ( ((b > 0 ) && (a > getMinMax(type, E_MAX) - b)) || ((b < 0) && (a < getMinMax(type, E_MIN) - b)) ) {
                throw OverUnderFlow("Addition failed: over/underflow");
            }
            const IOperand *io = _factory->createOperand( type, std::to_string( a + b ) );
            return io;
        }

        virtual IOperand const	*operator-( IOperand const & rhs ) const {
            double a = stod(this->toString());
            double b = stod(rhs.toString());
            eOperandType type = this->getType();
            if (rhs.getType() > type) {
                type = rhs.getType();
            }
           /*  std::cout << "a: " << a << " b: " << b << "\n";
            std::cout << "a - b = " << a - b << " max: " << getMinMax(type, E_MAX) << "\n"; */
            if ( ((b > 0 ) && (a < getMinMax(type, E_MIN) + b)) || ((b < 0) && (a > getMinMax(type, E_MAX) + b)) ) {
                throw OverUnderFlow("Subtraction failed: over/underflow");
            }
            const IOperand *io = _factory->createOperand( type, std::to_string(a - b) );
            return io;
        }

        virtual IOperand const	*operator*( IOperand const & rhs ) const {
            double a = stod(this->toString());
            double b = stod(rhs.toString());
            eOperandType type = this->getType();
            if (type <= 2) {
                if (rhs.getType() > type) {
                    type = rhs.getType();
                }
                if (a > 0 && b > 0) {
                    if (a > (getMinMax(type, E_MAX) / b)) {
                        throw OverUnderFlow("Multiplication failed: over/underflow");
                    }
                } else if (a > 0 && b < 0) {
                    if (b < (getMinMax(type, E_MIN) / a)) {
                        throw OverUnderFlow("Multiplication failed: over/underflow");
                    }
                } else if (a < 0 && b > 0) {
                    if (a < (getMinMax(type, E_MIN) / b)) {
                        throw OverUnderFlow("Multiplication failed: over/underflow");
                    }
                } else if (a < 0 && b < 0) {
                    if (b < (getMinMax(type, E_MAX) / a)) {
                        throw OverUnderFlow("Multiplication failed: over/underflow");
                    }
                }
            }
            const IOperand *io = _factory->createOperand( type, std::to_string(a * b) );
            return io;
        }

        virtual IOperand const	*operator/( IOperand const & rhs ) const {
            double i = stod(rhs.toString());
            if (i == 0) {
                throw DivByZero("Div by zero");
            }
            double t = stod(this->toString());
             eOperandType type = this->getType();
            if (rhs.getType() > type) {
                type = rhs.getType();
            }
            const IOperand *io = _factory->createOperand( type, std::to_string(t / i) );
            return io;
        }

        virtual IOperand const	*operator%( IOperand const & rhs ) const {
            double i = stoi(rhs.toString());
            if (i == 0) {
                throw DivByZero("Div by zero");
            }
            double t = stoi(this->toString());
            eOperandType type = this->getType();
            if (rhs.getType() > type) {
                type = rhs.getType();
            }
            const IOperand *io = _factory->createOperand( type, std::to_string( (T) fmod(t, i) ));
            std::cout << "fmod of " << i << " and " << t << " is: " << fmod(i,t) << "\n";
            return io;
        }

	    virtual const std::string   &toString(void) const{
            return *_stringValue;
        }

        T                   getValue() {
            return _numValue;
        }

    private:
        Operands();

        T                   _numValue;
        eOperandType        _e;
        const std::string   *_stringValue;
        OperandFactory      *_factory;

        double              getMinMax(eOperandType type, eMinMax maxTrue) const {
            switch (type) {
                case INT8: {
                    if (maxTrue)
                        return std::numeric_limits<int8_t>::max();
                    else
                        return std::numeric_limits<int8_t>::min();
                }
                case INT16: {
                    if (maxTrue)
                        return std::numeric_limits<int16_t>::max();
                    else
                        return std::numeric_limits<int16_t>::min();
                }
                case INT32: {
                    if (maxTrue)
                        return std::numeric_limits<int32_t>::max();
                    else
                        return std::numeric_limits<int32_t>::min();
                }
                case FLOAT: {
                    if (maxTrue)
                        return std::numeric_limits<float>::max();
                    else
                        return std::numeric_limits<float>::min();
                }
                case DOUBLE: {
                    if (maxTrue)
                        return std::numeric_limits<double>::max();
                    else
                        return std::numeric_limits<double>::min();
                }
                case MAX: {
                    throw DivByZero("Value not found");
                }
            }
        }
};

#endif
