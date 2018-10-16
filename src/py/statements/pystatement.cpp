#include "pystatement.hpp"
#include "../pyenvironment.hpp"

void PyStatement::evaluate() {
    PyEnvironment::Instance().parseStatement(expression);
}
