#include <utility>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>

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
    bool recursionEnds = false;

    for (auto &funcStatement : funcStatements) {
        try {
            std::shared_ptr<PyIfBlock> statement = std::dynamic_pointer_cast<PyIfBlock>(funcStatement);

            for (auto &ifStatement : statement->trueBlock) {
                try {
                    std::shared_ptr<PyReturn> temp = std::dynamic_pointer_cast<PyReturn>(ifStatement);

                    if (temp->expression.find(funcName) == std::string::npos) {
                        continue;
                    }
                    else {
                        std::string compExpr = statement->expression;
                        std::string retExpr = temp->expression;
                        boost::regex varNameReg{R"([A-Za-z_]+[A-Za-z0-9_]*)"};
                        boost::regex numReg{R"([0-9]+)"};
                        boost::regex regex{R"(^[A-Za-z_]+([\w]+)?([ ]+)?)"};
                        boost::smatch match;

                        std::string funcNameCop;
                        if (boost::regex_search(retExpr, match, regex)) {
                            funcNameCop = match.str();
                            boost::trim(funcNameCop);

                            std::string::size_type i = retExpr.find(funcNameCop);
                            if (i != std::string::npos) {
                                retExpr.erase(i,funcNameCop.size());
                            }
                        }

                        std::vector<std::string> funcInputs;

                        regex = R"(("[\w ,\.\-=+*\/!@#$%^&*()|{}\[\];\'<>?]+")|([A-Za-z_]+[A-Za-z0-9_]*)|([\d]+(\.[\d]+)?))";

                        for (boost::sregex_iterator i = boost::sregex_iterator(retExpr.begin(), retExpr.end(), regex);
                             i != boost::sregex_iterator(); ++i) {
                            boost::smatch m = *i;

                            funcInputs.push_back(m.str());
                        }

                        for (auto &funcInput : funcInputs) {
                            boost::trim(funcInput);
                            std::vector<std::string> split;
                            boost::split(split, funcInput, boost::is_any_of("+/-*"));
                            int var;
                            bool varLeft;
                            std::string vName;

                            for (const auto &j : split) {
                                if (boost::regex_search(j, match, varNameReg)) {
                                    vName = match.str();
                                    boost::trim(vName);
                                    break;
                                }
                            }

                            if (statement->expression.find(vName) == std::string::npos) {
                                continue;
                            }
                            else {
                                std::vector<std::string> tempV;
                                boost::split(tempV, statement->expression, boost::is_any_of(" =><"));
                                std::string n = tempV[0];
                                boost::trim(n);

                                varLeft = n.find(vName) != std::string::npos;
                            }

                            var = PyEnvironment::Instance().getVar(vName)->getData<int>();
                            PyEnvironment::Instance().parseStatement(statement->expression);

                            std::shared_ptr<PyObject> pyObject = PyEnvironment::Instance().exprContext.expressions[0]->evaluate();
                            auto iter = PyEnvironment::Instance().exprContext.expressions.begin();
                            PyEnvironment::Instance().exprContext.expressions.erase(iter, iter + 1);

                            if (pyObject->type == PyConstants::VarTypes::NUMBER) {
                                std::shared_ptr<PyInt> pyBool = std::dynamic_pointer_cast<PyInt>(pyObject);

                                while(!pyBool->getData<int>()) {
                                    std::stringstream pStatement;
                                    pStatement << vName << "=" << funcInput;
                                    PyEnvironment::Instance().parseStatement(pStatement.str());
                                    int comp = PyEnvironment::Instance().getVar(vName)->getData<int>();

                                    if (var == comp) {
                                        return false;
                                    }

                                    if (statement->expression.find('<') || statement->expression.find("<=")) {
                                        if (varLeft) {
                                            return var > comp;
                                        }
                                        else {
                                            return var < comp;
                                        }
                                    }
                                    else if (statement->expression.find('>') || statement->expression.find(">=")) {
                                        if (varLeft) {
                                            return var < comp;
                                        }
                                        else {
                                            return var > comp;
                                        }
                                    }
                                    else if (statement->expression.find("==")) {
                                        int ifExprVal;
                                        if (varLeft) {
                                            ifExprVal = std::stoi(split[1]);
                                        }
                                        else {
                                            ifExprVal = std::stoi(split[0]);
                                        }

                                        if ((ifExprVal > var && var > comp) || (ifExprVal < var && var < comp)) {
                                            return false;
                                        }
                                        else if ((ifExprVal > var && var < comp) || (ifExprVal < var && var > comp)) {
                                            return true;
                                        }
                                    }
                                }

                                return static_cast<bool>(pyBool->getData<int>());
                            }
                        }

                        return false;
                    }
                } catch ( ... ) {
                    continue;
                }
            }

            for (auto &ifStatement : statement->falseBlock) {
                try {
                    std::shared_ptr<PyReturn> temp = std::dynamic_pointer_cast<PyReturn>(ifStatement);

                    if (temp->expression.find(funcName) == std::string::npos) {
                        continue;
                    }
                    else {
                        std::string compExpr = statement->expression;
                        std::string retExpr = temp->expression;
                        boost::regex varNameReg{R"([A-Za-z_]+[A-Za-z0-9_]*)"};
                        boost::regex numReg{R"([0-9]+)"};
                        boost::regex regex{R"(^[A-Za-z_]+([\w]+)?([ ]+)?)"};
                        boost::smatch match;

                        std::string funcNameCop;
                        if (boost::regex_search(retExpr, match, regex)) {
                            funcNameCop = match.str();
                            boost::trim(funcNameCop);

                            std::string::size_type i = retExpr.find(funcNameCop);
                            if (i != std::string::npos) {
                                retExpr.erase(i,funcNameCop.size());
                            }
                        }

                        std::vector<std::string> funcInputs;

                        regex = R"(("[\w ,\.\-=+*\/!@#$%^&*()|{}\[\];\'<>?]+")|([A-Za-z_]+[A-Za-z0-9_]*)|([\d]+(\.[\d]+)?))";

                        for (boost::sregex_iterator i = boost::sregex_iterator(retExpr.begin(), retExpr.end(), regex);
                             i != boost::sregex_iterator(); ++i) {
                            boost::smatch m = *i;

                            funcInputs.push_back(m.str());
                        }

                        for (auto &funcInput : funcInputs) {
                            boost::trim(funcInput);
                            std::vector<std::string> split;
                            boost::split(split, funcInput, boost::is_any_of("+/-*"));
                            int var;
                            bool varLeft;
                            std::string vName;

                            for (const auto &j : split) {
                                if (boost::regex_search(j, match, varNameReg)) {
                                    vName = match.str();
                                    boost::trim(vName);
                                    break;
                                }
                            }

                            if (statement->expression.find(vName) == std::string::npos) {
                                continue;
                            }
                            else {
                                std::vector<std::string> tempV;
                                boost::split(tempV, statement->expression, boost::is_any_of(" =><"));
                                std::string n = tempV[0];
                                boost::trim(n);

                                varLeft = n.find(vName) != std::string::npos;
                            }

                            var = PyEnvironment::Instance().getVar(vName)->getData<int>();
                            PyEnvironment::Instance().parseStatement(statement->expression);

                            std::shared_ptr<PyObject> pyObject = PyEnvironment::Instance().exprContext.expressions[0]->evaluate();
                            auto iter = PyEnvironment::Instance().exprContext.expressions.begin();
                            PyEnvironment::Instance().exprContext.expressions.erase(iter, iter + 1);

                            if (pyObject->type == PyConstants::VarTypes::NUMBER) {
                                std::shared_ptr<PyInt> pyBool = std::dynamic_pointer_cast<PyInt>(pyObject);

                                while(!pyBool->getData<int>()) {
                                    std::stringstream pStatement;
                                    pStatement << vName << "=" << funcInput;
                                    PyEnvironment::Instance().parseStatement(pStatement.str());
                                    int comp = PyEnvironment::Instance().getVar(vName)->getData<int>();

                                    if (var == comp) {
                                        return false;
                                    }

                                    if (statement->expression.find('<') || statement->expression.find("<=")) {
                                        if (varLeft) {
                                            return var > comp;
                                        }
                                        else {
                                            return var < comp;
                                        }
                                    }
                                    else if (statement->expression.find('>') || statement->expression.find(">=")) {
                                        if (varLeft) {
                                            return var < comp;
                                        }
                                        else {
                                            return var > comp;
                                        }
                                    }
                                    else if (statement->expression.find("==")) {
                                        int ifExprVal;
                                        if (varLeft) {
                                            ifExprVal = std::stoi(split[1]);
                                        }
                                        else {
                                            ifExprVal = std::stoi(split[0]);
                                        }

                                        if ((ifExprVal > var && var > comp) || (ifExprVal < var && var < comp)) {
                                            return false;
                                        }
                                        else if ((ifExprVal > var && var < comp) || (ifExprVal < var && var > comp)) {
                                            return true;
                                        }
                                    }
                                }

                                return static_cast<bool>(pyBool->getData<int>());
                            }
                        }

                        return false;
                    }
                } catch ( ... ) {
                    continue;
                }
            }
        } catch ( ... ) {
            continue;
        }
    }
    return recursionEnds;
}


void PyFunction::evaluate(std::vector<std::string> args) {
    if (!parseSigToVars(std::move(args))) {
        PyEnvironment::Instance().flushFunctionStack();
        return;
    }

    /*if (PyEnvironment::Instance().isRecursive && !PyEnvironment::Instance().singleRecurseCheck) {
        PyEnvironment::Instance().isRecursive = false;
        PyEnvironment::Instance().singleRecurseCheck = true;

        if (!checkRecursionEnds()) {
            PyEnvironment::Instance().recursiveFunctionsEnd.push_back(false);
            PyEnvironment::Instance().flushFunctionStack();
            return;
        }

        PyEnvironment::Instance().recursiveFunctionsEnd.push_back(true);
    }*/

    for (auto &funcStatement : funcStatements) {
        if (PyEnvironment::Instance().funcReturn) {
            PyEnvironment::Instance().funcReturn = false;
            break;
        }

        funcStatement->evaluate();
    }

    // PyEnvironment::Instance().singleRecurseCheck = false;
}