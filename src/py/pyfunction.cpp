#include "pyfunction.hpp"

boost::any PyFunction::evaluate() {
    for (auto &funcStatement : funcStatements) {
        funcStatement->evaluate();
    }

    return boost::any();
}