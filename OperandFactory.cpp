#include "OperandFactory.hpp"
#include "Operands.hpp"


OperandFactory::OperandFactory() {
    OperandFactory::createFunctions[INT8] = &OperandFactory::createInt8;
    OperandFactory::createFunctions[INT16] = &OperandFactory::createInt16;
    OperandFactory::createFunctions[INT32] = &OperandFactory::createInt32;
    OperandFactory::createFunctions[FLOAT] = &OperandFactory::createFloat;
    OperandFactory::createFunctions[DOUBLE] = &OperandFactory::createDouble;
}

OperandFactory::~OperandFactory() {}

OperandFactory      &OperandFactory::operator=(OperandFactory const &rhs) {
    for (int i = 0; i < MAX; i ++) {
        createFunctions[i] = rhs.getFunctions()[i];
    }
    return *this;
}

OperandFactory::OperandFactory(OperandFactory const &src) {
    this->operator=(src);
}

const IOperand *OperandFactory::createOperand(eOperandType type, std::string const &value) const {
    return (this->*(createFunctions[type]))(value);
}

const OperandFactory::t_createFunctions   *OperandFactory::getFunctions() const {
    const t_createFunctions *temp = &createFunctions[0];
    return temp;
}

const IOperand * OperandFactory::createInt8( const std::string & value ) const {
    double d = stod(value);
    if (d > std::numeric_limits<int8_t>::max() || d < std::numeric_limits<int8_t>::min()) {
        throw OverUnderFlow("Can't create value: over/underflow");
    }
    const Operands<int8_t> *temp = new Operands<int8_t>(d, INT8);
    return temp;
}

const IOperand * OperandFactory::createInt16( const std::string & value ) const {
    double d = stod(value);
    if (d > std::numeric_limits<int16_t>::max() || d < std::numeric_limits<int16_t>::min()) {
        throw OverUnderFlow("Can't create value: over/underflow");
    }
    const Operands<int16_t> *temp = new Operands<int16_t>(d, INT16);
    return temp;
}

const IOperand * OperandFactory::createInt32( const std::string & value ) const {
    double d = stod(value);
    if (d > std::numeric_limits<int32_t>::max() || d < std::numeric_limits<int32_t>::min()) {
        throw OverUnderFlow("Can't create value: over/underflow");
    }
    const Operands<int32_t> *temp = new Operands<int32_t>(stod(value), INT32);
    return temp;
}

const IOperand * OperandFactory::createFloat( const std::string & value ) const {
    float d = stof(value);
    const Operands<float> *temp = new Operands<float>(d, FLOAT);
    return temp;
}

const IOperand * OperandFactory::createDouble( const std::string & value ) const {
    double d = stod(value);
    const Operands<double> *temp = new Operands<double>(d, DOUBLE);
    return temp;
}
