#include "pyifblock.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

#include "../pyobject.hpp"
#include "../pyenvironment.hpp"

PyIfBlock::PyIfBlock(std::string expr) : PyStatement() {
    boost::regex regex {R"((((   )+)?if )|[\w]( )*(==|<|>|<=|>=|and|or)( )*[\w])"};

    std::vector<std::string> results;

    for (boost::sregex_iterator i = boost::sregex_iterator(expr.begin(), expr.end(), regex);
         i != boost::sregex_iterator(); ++i) {
        boost::smatch m = *i;

        results.push_back(m.str());
    }

    expression = results[1];
    results.clear();

    std::stringstream ss(expr);
    std::string statement;
    while(std::getline(ss, statement, '\n')) {
        if (!statement.empty()) {
            boost::trim(statement);

            // TODO handle else stuff here

            std::unique_ptr<PyStatement> pyStatement = std::unique_ptr<PyStatement>(new PyStatement(statement));
            trueBlock.push_back(std::move(pyStatement));
        }
    }
}

void PyIfBlock::evaluate() {
    PyStatement::evaluate();

    std::shared_ptr<PyObject> pyObject = PyEnvironment::Instance().exprContext.expressions[0]->evaluate();
    auto iter = PyEnvironment::Instance().exprContext.expressions.begin();
    PyEnvironment::Instance().exprContext.expressions.erase(iter, iter + 1);

    if (pyObject->type == PyConstants::VarTypes::BOOL) {
        std::shared_ptr<PyBool> pyBool = std::dynamic_pointer_cast<PyBool>(pyObject);

        if (pyBool->getData<bool>()) {
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
