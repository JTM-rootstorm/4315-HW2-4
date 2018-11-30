#include <utility>

#ifndef MYPYTHON_PYFUNCTION_HPP
#define MYPYTHON_PYFUNCTION_HPP

#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>
#include <functional>

#include "pyobject.hpp"
#include "statements/pystatement.hpp"

class PyFunction {
public:
    PyFunction() = default;

    PyFunction(const PyFunction& other);

    std::string getName() const { return funcName; }

    std::function<void(std::vector<std::string>)> eval = [=](std::vector<std::string> args){
        PyFunction::evaluate(std::move(args));
    };

    bool isStdFunc = false;
protected:
    std::string funcName;
    std::vector<std::string> funcSigVars;
    std::vector<std::shared_ptr<PyStatement>> funcStatements;

    std::unordered_map<std::string, std::shared_ptr<PyObject>> localVars;

    bool parseSigToVars(std::vector<std::string> args);
    bool checkRecursionEnds() const;

    virtual void evaluate(std::vector<std::string> args);

    friend class PyFunctionBuilder;
    friend class FunctionModule;
};
#endif //MYPYTHON_PYFUNCTION_HPP
