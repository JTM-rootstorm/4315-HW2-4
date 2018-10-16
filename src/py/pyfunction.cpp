#include "pyfunction.hpp"
#include "pyenvironment.hpp"

void PyFunction::evaluate(std::vector<std::string> args) {
    PyEnvironment::Instance().localFuncStack.push(funcName);

    for (auto &funcStatement : funcStatements) {
        if (PyEnvironment::Instance().funcReturn) {
            PyEnvironment::Instance().funcReturn = false;
            return;
        }

        funcStatement->evaluate();
    }

    PyEnvironment::Instance().localFuncStack.pop();
}