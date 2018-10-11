#ifndef MYPYTHON_PYFUNCTIONBUILDER_HPP
#define MYPYTHON_PYFUNCTIONBUILDER_HPP

#include <memory>

#include "py/pyfunction.hpp"

class PyFunctionBuilder {
public:
    static PyFunctionBuilder& Instance() {
        static PyFunctionBuilder myInstance;

        return myInstance;
    }

    PyFunctionBuilder(PyFunctionBuilder const&) = delete;
    PyFunctionBuilder(PyFunctionBuilder&&) = delete;
    PyFunctionBuilder& operator=(PyFunctionBuilder const&) = delete;
    PyFunctionBuilder& operator=(PyFunctionBuilder &&) = delete;


protected:
    PyFunctionBuilder() = default;
    ~PyFunctionBuilder() = default;

private:
    std::unique_ptr<PyFunction> function;
};
#endif //MYPYTHON_PYFUNCTIONBUILDER_HPP
