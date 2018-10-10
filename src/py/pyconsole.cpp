//
// Created by mgjus on 10/9/2018.
//

#include "pyconsole.hpp"

#include <iostream>

void PyConsole::printToConsole(std::string &format) {
    std::cout << format << std::endl;
}

void PyConsole::logError(PyErrors::ERR_TYPE type, ...) {
    std::string errorMessage = PyErrors::Instance().getErrorMessage(type);

    va_list argptr;
    va_start(argptr, type);
    vfprintf(stderr, errorMessage.c_str(), argptr);
    va_end(argptr);
}
