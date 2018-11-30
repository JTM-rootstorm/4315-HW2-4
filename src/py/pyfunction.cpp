#include <utility>
#include <sstream>

#include "pyfunction.hpp"
#include "pyenvironment.hpp"
#include "statements/pyreturn.hpp"
#include "statements/pyifblock.hpp"

PyFunction::PyFunction(const PyFunction &other) {
    funcName = other.funcName;
    funcSigVars = other.funcSigVars;
    funcStatements = other.funcStatements;
    eval = other.eval;
    isStdFunc = other.isStdFunc;
}

bool PyFunction::parseSigToVars(std::vector<std::string> args) {
    if (args.size() == funcSigVars.size()) {
        std::stringstream varBuilder;

        for (unsigned int i = 0; i < funcSigVars.size(); i++) {
            varBuilder << funcSigVars[i] << "=" << args[i];
            std::string expr = varBuilder.str();
            PyEnvironment::Instance().parseStatement(expr);
        }

        return true;
    }
    std::stringstream ss;
    int missingArgs = static_cast<int>(funcSigVars.size() - args.size());

    for (unsigned long i = args.size(); i < funcSigVars.size(); i++) {
        if (i == funcSigVars.size() - 1) {
            ss << "and '" << funcSigVars[i] << "'";
            continue;
        }

        ss << "'" << funcSigVars[i] << "', ";
    }

    std::string missingArgNames = ss.str();

    PyEnvironment::Instance().pyConsole.logError(PyErrors::ERR_TYPE::TypeErrorFunction, (funcName + "()").c_str(),
            missingArgs, missingArgNames.c_str());

    return false;
}

bool PyFunction::checkRecursionEnds() const {
    for (auto &funcStatement : funcStatements) {
        try {
            std::shared_ptr<PyIfBlock> statement = std::dynamic_pointer_cast<PyIfBlock>(funcStatement);

            for (auto &ifStatement : statement->trueBlock) {
                try {
                    std::shared_ptr<PyReturn> temp = std::dynamic_pointer_cast<PyReturn>(ifStatement);
                    return true;
                } catch ( ... ) {
                    continue;
                }
            }

            for (auto &ifStatement : statement->falseBlock) {
                try {
                    std::shared_ptr<PyReturn> temp = std::dynamic_pointer_cast<PyReturn>(ifStatement);
                    return true;
                } catch ( ... ) {
                    continue;
                }
            }
        } catch ( ... ) {
            continue;
        }
    }
    return false;
}


void PyFunction::evaluate(std::vector<std::string> args) {
    if (!parseSigToVars(std::move(args))) {
        PyEnvironment::Instance().flushFunctionStack();
        return;
    }

    if (PyEnvironment::Instance().isRecursive) {
        PyEnvironment::Instance().isRecursive = false;
        PyEnvironment::Instance().flushFunctionStack();

        if (!checkRecursionEnds()) {
            PyEnvironment::Instance().recursiveFunctionsEnd.push_back(false);
            return;
        }

        PyEnvironment::Instance().recursiveFunctionsEnd.push_back(true);
        return;
    }

    for (auto &funcStatement : funcStatements) {
        if (PyEnvironment::Instance().funcReturn) {
            PyEnvironment::Instance().funcReturn = false;
            break;
        }

        funcStatement->evaluate();
    }
}