#include <utility>

#include "pyenvironment.hpp"

void PyEnvironment::setVar(const std::string &varName, boost::any value, PyConstants::VarTypes vartype) {
    if (localFuncStack.empty()) {
        setGlobalVar(varName, std::move(value), vartype);
        return;
    }
    std::vector<boost::any> localargs = {boost::any(vartype), boost::any(varName), value};

    modules.at("localFunc")->evaluate("setVar", localargs);
}

void PyEnvironment::setVar(const std::string &varName, PyObject &object) {
    if (localFuncStack.empty()) {
        setGlobalVar(varName, object);
        return;
    }
    modules.at("localFunc")->evaluate("setVar", varName, object);
}

std::shared_ptr<PyObject> PyEnvironment::getVar(const std::string &varName) {
    if (localFuncStack.empty()) return getGlobalVariable(varName);

    std::vector<boost::any> args{boost::any(varName)};
    modules.at("localFunc")->evaluate("getVar", args);

    std::shared_ptr<PyObject> pyObject = funcReturnStack.top();
    funcReturnStack.pop();

    return pyObject;
}

void PyEnvironment::runFunction(std::string funcSig) {
    std::vector<boost::any> args{boost::any(funcSig)};
    modules.at("localFunc")->evaluate("evalFunc", args);
}

void PyEnvironment::buildFunction() {
    modules.at("localFunc")->evaluate("gen", lexxerQueue);
    lexxerQueue.clear();
}

std::shared_ptr<PyObject> PyEnvironment::getGlobalVariable(const std::string &varName) {
    if (varNameUsed(varName)) {
        return globalVars.at(varName);
    } else {
        return nullptr;
    }
}

bool PyEnvironment::varNameUsed(std::string varName) {
    auto iter = globalVars.find(varName);

    return !(iter == globalVars.end());
}

void PyEnvironment::setGlobalVar(const std::string &varName, boost::any value,
                                 PyConstants::VarTypes vartype = PyConstants::VarTypes::NONE) {
    bool nameUsed = varNameUsed(varName);

    if (nameUsed && vartype != PyConstants::VarTypes::NONE) {
        if (globalVars.at(varName)->type == vartype) {
            modifyGlobalVar(vartype, varName, value);
        } else {
            mutateGlobalVar(vartype, varName, value);
        }
    } else if (!nameUsed && vartype != PyConstants::VarTypes::NONE) {
        createGlobalVar(vartype, varName, value);
    }
}

void PyEnvironment::setGlobalVar(const std::string &varName, PyObject &object) {
    bool nameUsed = varNameUsed(varName);

    if (nameUsed && object.type != PyConstants::VarTypes::NONE) {
        if (globalVars.at(varName)->type == object.type) {
            modifyGlobalVar(object.type, varName, object.getData<boost::any>());
        } else {
            mutateGlobalVar(object.type, varName, object.getData<boost::any>());
        }
    } else if (!nameUsed && object.type != PyConstants::VarTypes::NONE) {
        createGlobalVar(object.type, varName, object.getData<boost::any>());
    }
}

void
PyEnvironment::mutateGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value) {
    switch ( vartype ) {
        case PyConstants::VarTypes::NUMBER: {
            globalVars.at(varName) = std::make_shared<PyInt>(boost::any_cast<int>(value));
            break;
        }
        case PyConstants::VarTypes::BOOL: {
            globalVars.at(varName) = std::make_shared<PyBool>(boost::any_cast<bool>(value));
            break;
        }
        case PyConstants::VarTypes::STRING: {
            globalVars.at(varName) = std::make_shared<PyString>(boost::any_cast<std::string>(value));
            break;
        }
        default:
            break;
    }
}

void PyEnvironment::modifyGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value) {
    switch ( vartype ) {
        case PyConstants::VarTypes::NUMBER: {
            globalVars.at(varName)->setData<int>(boost::any_cast<int>(value));
            break;
        }
        case PyConstants::VarTypes::BOOL: {
            globalVars.at(varName)->setData<bool>(boost::any_cast<bool>(value));
            break;
        }
        case PyConstants::VarTypes::STRING: {
            globalVars.at(varName)->setData<std::string>(boost::any_cast<std::string>(value));
            break;
        }
        default:
            break;
    }
}

void PyEnvironment::createGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value) {
    switch ( vartype ) {
        case PyConstants::VarTypes::NUMBER: {
            globalVars.insert(
                    std::make_pair(
                            varName,
                            std::make_shared<PyInt>(boost::any_cast<int>(value))
                    ));
            break;
        }
        case PyConstants::VarTypes::BOOL: {
            globalVars.insert(
                    std::make_pair(
                            varName,
                            std::make_shared<PyBool>(boost::any_cast<bool>(value))
                    ));
            break;
        }
        case PyConstants::VarTypes::STRING: {
            globalVars.insert(
                    std::make_pair(
                            varName,
                            std::make_shared<PyString>(boost::any_cast<std::string>(value))
                    ));
            break;
        }
        default:
            break;
    }
}

PyEnvironment::PyEnvironment() {
    modules.emplace("localFunc", std::unique_ptr<PyModule>(new FunctionModule()));
}

PyEnvironment::~PyEnvironment() {
    while (!localFuncStack.empty()) {
        localFuncStack.pop();
    }
    while (!funcReturnStack.empty()) {
        funcReturnStack.pop();
    }

    lexxerQueue.clear();

    modules.clear();
    globalVars.clear();
}

void PyEnvironment::parseStatement(const std::string &expression) {
    Driver driver;
    driver.parse_string(expression);
}
