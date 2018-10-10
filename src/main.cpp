#include <iostream>

#include "py/pyenvironment.hpp"

int main(int argc, char** argv) {

    PyFunction pyFunction("print", "\"BIG,- HATE\", \"woopty\", scoop3-8");
    pyFunction.parseFunctionHeader();

    return EXIT_SUCCESS;
}