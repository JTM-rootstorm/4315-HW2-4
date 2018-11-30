#include "pyifblock.hpp"

#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "../pyobject.hpp"
#include "../pyenvironment.hpp"
#include "pyreturn.hpp"

PyIfBlock::PyIfBlock(std::string expr) : PyStatement() {
    boost::regex regex {R"((((   )+)?if )|[\w]+([ ]+)?(==|!=|<|>|<=|>=|and|or)([ ]+)?[\w]+:\n)"};
    boost::replace_all(expr, "\r", "");
    std::vector<std::string> results;

    for (boost::sregex_iterator i = boost::sregex_iterator(expr.begin(), expr.end(), regex);
         i != boost::sregex_iterator(); ++i) {
        boost::smatch m = *i;

        results.push_back(m.str());
    }

    if (results.size() == 2) {
        expression = results[1];
    }
    else if (results.size() == 1) {
        expression = results[0];
    }
    else {
        return;
    }

    results.clear();

    // get rid of the : and newline
    expression.erase(expression.end() - 2, expression.end());
    boost::trim(expr);
    regex = R"(((   .+)((\n)?)+)+)";
    std::stringstream ss;
    for (boost::sregex_iterator i = boost::sregex_iterator(expr.begin(), expr.end(), regex);
         i != boost::sregex_iterator(); ++i) {
        boost::smatch m = *i;
        ss << m.str();
    }

    std::string statement;
    bool elseTrigger = false;
    while(std::getline(ss, statement, '\n')) {
        if (!statement.empty()) {
            boost::trim(statement);
            if(statement.find("else") != std::string::npos) {
                elseTrigger = true;
                continue;
            }

            if (!elseTrigger) {
                std::shared_ptr<PyStatement> pyStatement;

                if (statement.find("return") != std::string::npos) {
                    pyStatement = std::make_shared<PyReturn>(statement);
                }
                else if (statement.find("if") != std::string::npos) {
                    // pyStatement = std::unique_ptr<PyIfBlock>(new PyIfBlock(statement));
                }
                else {
                    pyStatement = std::make_shared<PyStatement>(statement);
                }

                trueBlock.push_back(std::move(pyStatement));
            }
            else {
                std::shared_ptr<PyStatement> pyStatement;

                if (statement.find("return") != std::string::npos) {
                    pyStatement = std::make_shared<PyReturn>(statement);
                }
                else {
                    pyStatement = std::make_shared<PyStatement>(statement);
                }

                falseBlock.push_back(std::move(pyStatement));
            }
        }
    }
}

void PyIfBlock::evaluate() {
    PyStatement::evaluate();

    std::shared_ptr<PyObject> pyObject = PyEnvironment::Instance().exprContext.expressions[0]->evaluate();
    auto iter = PyEnvironment::Instance().exprContext.expressions.begin();
    PyEnvironment::Instance().exprContext.expressions.erase(iter, iter + 1);

    if (pyObject->type == PyConstants::VarTypes::NUMBER) {
        std::shared_ptr<PyInt> pyBool = std::dynamic_pointer_cast<PyInt>(pyObject);

        if (pyBool->getData<int>()) {
            for (auto &i : trueBlock) {
                i->evaluate();
            }
        }
        else {
            for (auto &i : falseBlock) {
                i->evaluate();
            }
        }
    }
}
