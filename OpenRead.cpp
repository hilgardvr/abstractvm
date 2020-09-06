#include "OpenRead.hpp"


//  ******** InvalidOperation Exception ********

OpenRead::InvalidOperation::InvalidOperation() {}

OpenRead::InvalidOperation::~InvalidOperation() throw() {}

OpenRead::InvalidOperation  &OpenRead::InvalidOperation::operator=(InvalidOperation const &rhs) {
    if (this != &rhs) {
        std::exception::operator=(rhs);
    }
    return *this;
}

OpenRead::InvalidOperation::InvalidOperation(InvalidOperation const &src) {
    operator=(src);
}

const char* OpenRead::InvalidOperation::what() const throw() {
    return ("Invalid operation");
}

//  ********** OpenRead **************

OpenRead::OpenRead(std::string file) : _filename(file) {
    //std::cout << "OpenRead constructor called with " << file << std::endl;
    _factory = new OperandFactory();
}

OpenRead::OpenRead(OpenRead const &openRead) {
    //std::cout << "OpenRead copy constructor with " << openRead.getFilename() << std::endl;
    _factory = openRead._factory;
}

OpenRead::~OpenRead() {
    //std::cout << "OpenRead destructor called\n";
    delete _factory;
    size_t stackSize = stackVector.size();
    for (unsigned int i = 0; i < stackSize; i++) {
        delete stackVector[i];
    }
}

OpenRead    &OpenRead::operator=(OpenRead const &openRead) {
    this->_filename = openRead.getFilename();
    return *this;
}

std::string OpenRead::getFilename() const {
    return this->_filename;
}

void	OpenRead::readFile() {

    std::string operationStr;
    std::ifstream _inFile(_filename);

    while (1) {
        getline(_inFile, operationStr);
        if (!_inFile) {
            std::cout << "End of file reached.. exiting\n";
            break;
        }
        if (operationStr.compare(";;") == 0 || operationStr.compare("exit") == 0 ) {
            std::cout << "End of input reached.. exiting\n";
            return;
        } else if (operationStr.empty() || (!operationStr.empty() && operationStr[0] == ';') ) {
            continue;
        } else { 
            //remove comments from string
            size_t pos = operationStr.find_first_of(";");
            if (pos != std::string::npos) {
                operationStr = operationStr.substr(0, pos);
            }
            std::vector<std::string> strs;
            strs.clear();
            std::istringstream ss(operationStr);
            std::string item;

            while (std::getline(ss, item, ' ')) {
                strs.push_back(item);
            }

            if (!strs[0].empty()) {
                if (!checkOperation(strs)) {
                    break;
                }
            }
        }
    }
}


void    OpenRead::doOperation(std::vector<std::string> strs) {
    if (stackVector.size() < 2) {
        throw OpenRead::InvalidOperation();
    }
    const IOperand *io1 = stackVector.back();
    stackVector.pop_back();
    const IOperand *io2 = stackVector.back();
    stackVector.pop_back();
    const IOperand *newOper = NULL;
    try {
        if (strs[0].compare("add") == 0) {
            newOper = *io2 + *io1;
        } else if (strs[0].compare("sub") == 0) {
            newOper = *io2 - *io1;
        } else if (strs[0].compare("mul") == 0) {
            newOper = *io2 * *io1;
        } else if (strs[0].compare("div") == 0) {
            newOper = *io2 / *io1;
        } else if (strs[0].compare("mod") == 0) {
            newOper = *io2 % *io1;
        }
    } catch (DivByZero ex) {
        throw DivByZero("Div by zero");
    } catch (OverUnderFlow ex) {
        throw OverUnderFlow("Over/Under flow");
    }
    if (newOper != NULL)
        stackVector.push_back(newOper);
    delete io1;
    delete io2;
}

eOperandType    OpenRead::findType(std::string typeString) {
    if (typeString == "int8") {
        return INT8;
    } else if (typeString == "int16") {
        return INT16;
    } else if (typeString == "int32") {
        return INT32;
    } else if (typeString == "float") {
        return FLOAT;
    } else if (typeString == "double") {
        return DOUBLE;
    } else {
        return MAX;
    }
}

bool    OpenRead::checkOperation(std::vector<std::string> strs) {
    if (!strs[0].empty()) {
        if ( ( strs[0].compare("push") == 0 || strs[0].compare("assert") == 0 ) && strs.size() == 2 ) {
            int first = strs[1].find('(');
            int second = strs[1].find(')');
            if (first == std::string::npos || second == std::string::npos) {
                std::cerr << "Bracket not found - Peacefully exiting\n";
                return false;
            }
            std::string operand = strs[1].substr( 0, first );
            std::string strValue = strs[1].substr( (first + 1), (second - first - 1) );
            eOperandType operandType = findType(operand);
            if (operandType == MAX) {
                std::cerr << "Invalid operation - Peacefully exiting\n";
                return false;
            }
            if (strs[0].compare("push") == 0) {
                try {
                    stackVector.push_back(_factory->createOperand(operandType, strValue));
                } catch (OverUnderFlow ex) {
                    std::cerr << ex.what() << " - Peacefully exiting\n";
                    return false;
                }
            } else if (strs[0].compare("assert") == 0) {
                try {
                    assertValue(operandType, strValue);
                } catch (OpenRead::InvalidOperation ex) {
                    std::cerr << ex.what() << " - Peacefully exiting\n";
                    return false;
                }
            }
        } else if ( (strs[0].compare("add") == 0 || strs[0].compare("sub") == 0
        || strs[0].compare("mul") == 0 || strs[0].compare("div") == 0 || strs[0].compare("mod") == 0) && strs.size() == 1 ) {
            try {
                doOperation(strs);
            } catch (OpenRead::InvalidOperation) {
                std::cout << "Error: less than 2 elements on the stack - Peacefully exiting\n";
                return false;
            } catch (DivByZero ex) {
                std::cerr << ex.what() << " - Peacefully exiting\n";
                return false;
            } catch (OverUnderFlow ex) {
                std::cerr << ex.what() << " - Peacefully exiting\n";
                return false;
            }
        } else if (strs[0].compare("pop") == 0) {
            if (stackVector.size() == 0) {
                std::cerr << "Error - Can't pop - Empty stack - Peacefully exiting\n";
                return false;
            }
            std::cout << "Popping: " << stackVector.back()->toString() << "\n";
            stackVector.pop_back();
        } else if (strs[0].compare("dump") == 0) {
            std::cout << "Dumping stack:\n";
            size_t size = stackVector.size();
            for (size_t i = size; i > 0; i--) {
                std::cout << stackVector[i - 1]->toString() << "\n";
            }
        } else if (strs[0].compare("print") == 0) {
            try {
                printValue();
            } catch (OpenRead::InvalidOperation ex) {
                std::cerr << ex.what() << " - Peacefully exiting\n";
                return false;
            }
        } else {
            std::cerr << "Invalid command - " << strs[0] << " - Peacefully exiting\n";
            return false;
        }
    }
    return true;
}

void    OpenRead::assertValue(eOperandType type, std::string strValue) {
    const IOperand *io = stackVector.back();
    const IOperand *temp = _factory->createOperand(type, strValue);
    if (io->getType() != type || io->toString().compare(temp->toString()) != 0 ) {
        //std::cerr << io->toString() << " " << temp->toString() << " assert failed\n";
        std::cerr << "assert failed\n";
        throw OpenRead::InvalidOperation();
    } else {
        std::cout << "assert passed\n";
    }
    delete temp;
}

void    OpenRead::printValue() {
    const IOperand *io = stackVector.back();
    if (io->getType() != INT8) {
        std::cerr << io->toString() << " " << " print failed - not an int8\n";
        throw OpenRead::InvalidOperation();
    } else {
        char c = std::stoi(io->toString());
        std::cout << c << "\n";
    }
}

