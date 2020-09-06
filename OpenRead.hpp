#ifndef __OPENREAD_HPP
# define __OPENREAD_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "IOperand.hpp"
#include "eOperandType.hpp"
#include "OperandFactory.hpp"
#include "DivByZero.hpp"
#include "OverUnderFlow.hpp"

class OpenRead {

public:

    class InvalidOperation : public std::exception {
    public:
        InvalidOperation();
        ~InvalidOperation() throw();
        InvalidOperation    &operator=(InvalidOperation const &rhs);
        InvalidOperation(InvalidOperation const &src);

        virtual const char* what() const throw();
    };

    OpenRead(std::string file);
    ~OpenRead();
    OpenRead(OpenRead const &openRead);
    OpenRead    &operator=(OpenRead const &openRead);

    IOperand const  *createOperand(eOperandType type, std::string const &value) const;

    std::string     getFilename() const;
    void            readFile();
    bool            checkOperation(std::vector<std::string> strs);
    eOperandType    findType(std::string typeString);
    int             findBiggestType(IOperand *io1, IOperand *io2);
    void            doOperation(std::vector<std::string> strs);

private:
    OpenRead();
    void            assertValue(eOperandType type, std::string strValue);
    void            printValue();

    const OperandFactory    *_factory;

    std::string _filename;
    std::vector<const IOperand*> stackVector;
};

#endif
