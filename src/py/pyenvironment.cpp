#include <utility>

#include "pyenvironment.hpp"

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

PyObject * PyEnvironment::getGlobalVariable(const std::string &varName) {
    if (varNameUsed(varName)) {
        return globalVars.at(varName).get();
    } else {
        return nullptr;
    }
}

bool PyEnvironment::varNameUsed(std::string varName) {
    auto iter = globalVars.find(varName);

    return !(iter == globalVars.end());
}

void
PyEnvironment::mutateGlobalVar(PyConstants::VarTypes vartype, const std::string &varName, boost::any value) {
    switch ( vartype ) {
        case PyConstants::VarTypes::NUMBER: {
            std::unique_ptr<PyInt> pyInt = std::unique_ptr<PyInt>(new PyInt(boost::any_cast<int>(value)));
            globalVars.at(varName) = std::move(pyInt);
            break;
        }
        case PyConstants::VarTypes::BOOL: {
            std::unique_ptr<PyBool> pyBool = std::unique_ptr<PyBool>(new PyBool(boost::any_cast<bool>(value)));
            globalVars.at(varName) = std::move(pyBool);
            break;
        }
        case PyConstants::VarTypes::STRING: {
            std::unique_ptr<PyString> pyString = std::unique_ptr<PyString>(new PyString(boost::any_cast<std::string>(value)));
            globalVars.at(varName) = std::move(pyString);
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
                            std::unique_ptr<PyInt>(new PyInt(boost::any_cast<int>(value)))
                    ));
            break;
        }
        case PyConstants::VarTypes::BOOL: {
            globalVars.insert(
                    std::make_pair(
                            varName,
                            std::unique_ptr<PyBool>(new PyBool(boost::any_cast<bool>(value)))
                    ));
            break;
        }
        case PyConstants::VarTypes::STRING: {
            globalVars.insert(
                    std::make_pair(
                            varName,
                            std::unique_ptr<PyString>(new PyString(boost::any_cast<std::string>(value)))
                    ));
            break;
        }
        default:
            break;
    }
}
