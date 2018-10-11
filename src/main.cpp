

#include "py/pyfunction.hpp"

int main(int argc, char** argv) {

    PyFunction pyFunction("print", "(\"BIG,- HATE\", \"woopty\", scoop38, 0)");
    pyFunction.parseFunctionHeader();

    return EXIT_SUCCESS;
}