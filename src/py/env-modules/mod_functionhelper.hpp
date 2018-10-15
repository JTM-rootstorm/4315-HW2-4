#ifndef MYPYTHON_MOD_FUNCTIONHELPER_HPP
#define MYPYTHON_MOD_FUNCTIONHELPER_HPP

#include <unordered_map>

#include "../pyconstants.hpp"
#include "../pyfunctionbuilder.hpp"
#include "pymodule.hpp"

class PyFunction;

class FunctionModule : public PyModule {
public:
    FunctionModule() : PyModule() {}

    void evaluate(std::string func, std::vector<boost::any> args) override;
    void evaluate(std::string func, PyObject &object) override;

private:
    std::unordered_map<std::string, std::unique_ptr<PyFunction>> pyFunctions;

    PyFunctionBuilder pyFunctionBuilder;

    bool varNameUsed(std::string varName);

    void generateFunction(std::vector<boost::any> funcStrings);
    void setFunctionVar(const std::string &funcName, std::vector<boost::any> args);

    void mutateFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype, const std::string &varName,
                               boost::any value);
    void modifyFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype, const std::string &varName,
                               boost::any value);
    void createFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype, const std::string &varName,
                               boost::any value);
};

#endif //MYPYTHON_MOD_FUNCTIONHELPER_HPP
