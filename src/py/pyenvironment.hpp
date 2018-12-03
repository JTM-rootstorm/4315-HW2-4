#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <queue>
#include <stack>

#include "../boolexpr.hpp"
#include "../driver.hpp"

#include "pyconsole.hpp"
#include "pyfunctionbuilder.hpp"
#include "pyfunction.hpp"

#include "env-modules/mod_functionhelper.hpp"

using namespace Py;

class PyEnvironment {
public:
    static PyEnvironment& Instance() {
        static PyEnvironment myInstance;

        return myInstance;
    }

    PyEnvironment(PyEnvironment const&) = delete;
    PyEnvironment(PyEnvironment&&) = delete;
    PyEnvironment& operator=(PyEnvironment const&) = delete;
    PyEnvironment& operator=(PyEnvironment &&) = delete;

    PyConsole pyConsole;
    ExprContext exprContext;

    void setVar(const std::string &varName, boost::any value, PyConstants::VarTypes vartype);
    std::shared_ptr<PyObject> getVar(const std::string &varName);

    void buildFunction();
    void runFunction(std::string funcSig);
    void parseStatement(const std::string &expression);
    void parseFile(const std::string &fileName);

    void constructMainIf();

    void pushOntoFunctionStack(std::shared_ptr<PyFunction> function);
    std::shared_ptr<PyFunction> getFunctionStackTop();
    void popFunctionStack();
    void flushFunctionStack();
    bool funcStackEmpty();

    std::stack<std::shared_ptr<PyObject>> funcReturnStack;
    std::vector<boost::any> lexxerQueue;
    std::vector<int> nestedIfCount;
    std::vector<std::string> mutatedVars;
    std::vector<std::pair<std::string, bool>> recursiveFunctionsEnd;
    bool funcReturn = false;
    bool isRecursive = false;
    bool killRecursion = false;
    // bool singleRecurseCheck = false;

    int maxFuncStackSize = 250;

protected:
    PyEnvironment();
    ~PyEnvironment();

private:
    std::unordered_map<std::string, std::unique_ptr<PyModule>> modules;

    std::unordered_map<std::string, std::shared_ptr<PyObject>> globalVars;

    std::stack<std::shared_ptr<PyFunction>> funcStack;

    std::shared_ptr<PyObject> getGlobalVariable(const std::string &varName);

    bool varNameUsed(std::string varName);

    void setGlobalVar(const std::string &varName, boost::any value, PyConstants::VarTypes vartype);

    void mutateGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value);
    void modifyGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value);
    void createGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value);


};