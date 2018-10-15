#include "pystatement.hpp"
#include "pyenvironment.hpp"

void PyStatement::evaluate() {
    PyEnvironment::Instance().pyDriver.parse_string(statement);
}
