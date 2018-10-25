#include "pyfunction.hpp"
#include "pyenvironment.hpp"

PyFunction::PyFunction(const PyFunction &other) {
    funcName = other.funcName;
    funcSigVars = other.funcSigVars;
    funcStatements = other.funcStatements;
    eval = other.eval;
    isStdFunc = other.isStdFunc;
}

void PyFunction::evaluate(std::vector<std::string> args) {
    // PyEnvironment::Instance().localFuncStack.push(funcName);

    for (auto &funcStatement : funcStatements) {
        if (PyEnvironment::Instance().funcReturn) {
            PyEnvironment::Instance().funcReturn = false;
            break;
        }

        funcStatement->evaluate();
    }

    // PyEnvironment::Instance().localFuncStack.pop();
}