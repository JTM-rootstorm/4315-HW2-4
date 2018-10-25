#ifndef MYPYTHON_PYFUNCTIONBUILDER_HPP
#define MYPYTHON_PYFUNCTIONBUILDER_HPP

#include <memory>
#include <string>

class PyFunction;

class PyFunctionBuilder {
public:
    PyFunctionBuilder() = default;

    PyFunctionBuilder & init();
    PyFunctionBuilder & parseFunctionSignature(std::string sig);
    PyFunctionBuilder & parseStatements(std::string funcBody);
    std::shared_ptr<PyFunction> build();

private:
    std::shared_ptr<PyFunction> function = nullptr;
};

#endif //MYPYTHON_PYFUNCTIONBUILDER_HPP
