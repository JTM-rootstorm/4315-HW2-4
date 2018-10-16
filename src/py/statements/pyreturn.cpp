#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

#include "pyreturn.hpp"
#include "../pyobject.hpp"
#include "../pyenvironment.hpp"

PyReturn::PyReturn(std::string expr) : PyStatement() {
    boost::regex regex{R"(^return[ ]+)"};
    boost::smatch match;

    std::string trash;
    if (boost::regex_search(expr, match, regex)) {
        trash = match.str();

        std::string::size_type i = expr.find(trash);
        if (i != std::string::npos) {
            expr.erase(i, trash.size());
        }
    }
    boost::trim(expr);
    expression = expr;
}

void PyReturn::evaluate() {
    PyStatement::evaluate();

    std::shared_ptr<PyObject> pyObject = PyEnvironment::Instance().exprContext.expressions[0]->evaluate();
    auto iter = PyEnvironment::Instance().exprContext.expressions.begin();
    PyEnvironment::Instance().exprContext.expressions.erase(iter, iter + 1);

    PyEnvironment::Instance().funcReturnStack.push(pyObject);
    PyEnvironment::Instance().funcReturn = true;
}
