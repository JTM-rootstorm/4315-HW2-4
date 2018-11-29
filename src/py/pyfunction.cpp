#include <utility>

#include "pyfunction.hpp"
#include "pyenvironment.hpp"

PyFunction::PyFunction(const PyFunction &other) {
    funcName = other.funcName;
    funcSigVars = other.funcSigVars;
    funcStatements = other.funcStatements;
    eval = other.eval;
    isStdFunc = other.isStdFunc;
}

bool PyFunction::parseSigToVars(std::vector<std::string> args) {
    if (args.size() == funcSigVars.size()) {
        for (unsigned int i = 0; i < funcSigVars.size(); i++) {
            std::string expr = funcSigVars[i] + "=" + args[i];
            PyEnvironment::Instance().parseStatement(expr);
        }

        return true;
    }

    return false;
}

void PyFunction::evaluate(std::vector<std::string> args) {
    // PyEnvironment::Instance().localFuncStack.push(funcName);
    if (!parseSigToVars(std::move(args))) {
        return;
    }

    for (auto &funcStatement : funcStatements) {
        if (PyEnvironment::Instance().funcReturn) {
            PyEnvironment::Instance().funcReturn = false;
            break;
        }

        funcStatement->evaluate();
    }

    // PyEnvironment::Instance().localFuncStack.pop();
}