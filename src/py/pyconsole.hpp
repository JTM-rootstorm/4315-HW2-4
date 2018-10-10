//
// Created by mgjus on 10/4/2018.
//

#pragma once

#include <string>
#include <cstdarg>

#include "pyerrors.hpp"

class PyConsole {
public:
    PyConsole() = default;
    void printToConsole(std::string &format);

    void logError(PyErrors::ERR_TYPE type, ...);
};