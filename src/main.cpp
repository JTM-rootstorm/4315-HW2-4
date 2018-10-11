#include <iostream>

#include "py/pyenvironment.hpp"

int main(int argc, char** argv) {

    /*PyFunction pyFunction("print", "(\"BIG,- HATE\", \"woopty\", scoop38, 0)");
    pyFunction.parseFunctionHeader();*/

    std::shared_ptr<PyObject> object = std::make_shared<PyInt>(7);

    std::cout << object->getData<int>() << std::endl;

    return EXIT_SUCCESS;
}