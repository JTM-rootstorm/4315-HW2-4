#ifndef MYPYTHON_MOD_FUNCTIONHELPER_HPP
#define MYPYTHON_MOD_FUNCTIONHELPER_HPP

#include <unordered_map>

#include "../pyconstants.hpp"
#include "../pyfunctionbuilder.hpp"
#include "pymodule.hpp"
#include "../pyfunction.hpp"


class PyFunction;

class FunctionModule : public PyModule {
public:
    FunctionModule() : PyModule() {
        initStandardFunctions();
    }
    ~FunctionModule() = default;

    void evaluate(const std::string &task, std::vector<boost::any> args) override;
    void evaluate(const std::string &func, const std::string &varName, PyObject &object) override;

private:
    std::unordered_map<std::string, std::shared_ptr<PyFunction>> pyFunctions;

    PyFunctionBuilder pyFunctionBuilder;

    bool varNameUsed(std::string varName);

    void generateFunction(std::vector<boost::any> funcStrings);
    void getFunctionVar(const std::string &funcName, const std::string &varName);
    void setFunctionVar(std::shared_ptr<PyFunction> function, std::vector<boost::any> args);
    void setFunctionVar(std::shared_ptr<PyFunction> function, const std::string &varName, PyObject &object);

    void mutateFunctionVar(std::shared_ptr<PyFunction> function, PyConstants::VarTypes vartype,
                           const std::string &varName,
                           boost::any value);
    void modifyFunctionVar(std::shared_ptr<PyFunction> function, PyConstants::VarTypes vartype,
                           const std::string &varName,
                           boost::any value);
    void createFunctionVar(std::shared_ptr<PyFunction> function, PyConstants::VarTypes vartype,
                           const std::string &varName,
                           boost::any value);

    void runFunction(const std::string &sig);

    void initStandardFunctions();
};

#endif //MYPYTHON_MOD_FUNCTIONHELPER_HPP
