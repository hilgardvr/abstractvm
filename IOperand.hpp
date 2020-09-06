#ifndef __IOPERAND_HPP
# define __IOPERAND_HPP

#include "eOperandType.hpp"
#include <string>

class IOperand {

public:

	virtual int				getPrecision(void) const = 0;					// Precision of the type of instance
	virtual eOperandType	getType(void) const = 0;						// Type of the instance
	
	virtual IOperand const	*operator+( IOperand const & rhs ) const = 0;	// Sum
	virtual IOperand const	*operator-( IOperand const & rhs ) const = 0;	// Difference
	virtual IOperand const	*operator*( IOperand const & rhs ) const = 0;	// Product
	virtual IOperand const	*operator/( IOperand const & rhs ) const = 0;	// Quocient
	virtual IOperand const	*operator%( IOperand const & rhs ) const = 0;	// Modulo

	virtual const std::string   &toString(void) const = 0;					// String representation of the instance

	virtual ~IOperand(void) {}
};

#endif
