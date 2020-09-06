#ifndef __OPERANDFACTORY_HPP
# define __OPERANDFACTORY_HPP

#include "IOperand.hpp"
#include "eOperandType.hpp"
#include "DivByZero.hpp"
#include "OverUnderFlow.hpp"

class OperandFactory {
    public:
        OperandFactory();
        ~OperandFactory();
        OperandFactory      &operator=(OperandFactory const &rhs);
        OperandFactory(OperandFactory const &src);

        const IOperand *createOperand(eOperandType type, std::string const &value) const;

    private:

        const IOperand *createInt8( const std::string & value ) const;
        const IOperand *createInt16( const std::string & value ) const;
        const IOperand *createInt32( const std::string & value ) const;
        const IOperand *createFloat( const std::string & value ) const;
        const IOperand *createDouble( const std::string & value ) const;

        typedef const IOperand *(OperandFactory::*t_createFunctions)(const std::string &) const;
        t_createFunctions   createFunctions[MAX];
        const t_createFunctions   *getFunctions() const;

};

#endif
