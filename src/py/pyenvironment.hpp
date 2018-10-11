//
// Created by mgjus on 10/4/2018.
//

#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "../boolexpr.hpp"
#include "pyconsole.hpp"
#include "pyfunction.hpp"

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

    void setGlobalVar(const std::string &varName, boost::any value, PyConstants::VarTypes vartype);
    void setGlobalVar(const std::string &varName, PyObject &object);
    PyObject * getGlobalVariable(const std::string &varName);

protected:
    PyEnvironment() = default;
    ~PyEnvironment() = default;

private:
    typedef std::pair<PyConstants::VarTypes, boost::any> PyFuncReturn;

    std::unordered_map<std::string, std::unique_ptr<PyObject>> globalVars;
    std::unordered_map<std::string, std::unique_ptr<PyFunction>> pyFunctions;

    bool varNameUsed(std::string varName);

    void mutateGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value);
    void modifyGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value);
    void createGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value);
};