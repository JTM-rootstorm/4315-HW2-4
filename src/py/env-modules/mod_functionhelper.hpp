#ifndef MYPYTHON_MOD_FUNCTIONHELPER_HPP
#define MYPYTHON_MOD_FUNCTIONHELPER_HPP

#include <unordered_map>

#include "../pyconstants.hpp"
#include "../pyfunctionbuilder.hpp"
#include "pymodule.hpp"

class PyFunction;

class FunctionModule : public PyModule {
public:
    FunctionModule() : PyModule() {
        initStandardFunctions();
    }
    ~FunctionModule() = default;

    void evaluate(const std::string &func, std::vector<boost::any> args) override;
    void evaluate(const std::string &func, const std::string &varName, PyObject &object) override;

private:
    std::unordered_map<std::string, std::unique_ptr<PyFunction>> pyFunctions;

    PyFunctionBuilder pyFunctionBuilder;

    bool varNameUsed(std::string varName);

    void generateFunction(std::vector<boost::any> funcStrings);
    void getFunctionVar(const std::string &funcName, const std::string &varName);
    void setFunctionVar(const std::string &funcName, std::vector<boost::any> args);
    void setFunctionVar(const std::string &funcName, const std::string &varName, PyObject &object);

    void mutateFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype, const std::string &varName,
                               boost::any value);
    void modifyFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype, const std::string &varName,
                               boost::any value);
    void createFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype, const std::string &varName,
                               boost::any value);

    void runFunction(const std::string &sig);

    void initStandardFunctions();
};

#endif //MYPYTHON_MOD_FUNCTIONHELPER_HPP
