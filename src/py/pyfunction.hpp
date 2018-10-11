#ifndef MYPYTHON_PYFUNCTION_HPP
#define MYPYTHON_PYFUNCTION_HPP

#include <string>
#include <utility>
#include <vector>

#include "pyobject.hpp"

/* This one is specifically for python's built-in functions like print */
class PyFunction {
public:
    explicit PyFunction(std::string _funcName, std::string _funcSignature) : funcName(std::move(_funcName)),
        funcSignature(std::move(_funcSignature)) {};

    void parseFunctionHeader();
private:
    static void PyPrint(std::vector<std::string> args);

    std::string funcName;
    std::string funcSignature;
};
#endif //MYPYTHON_PYFUNCTION_HPP
