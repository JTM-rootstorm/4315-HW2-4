#include "mod_functionhelper.hpp"

#include "../pyenvironment.hpp"

void FunctionModule::evaluate(std::string func, std::vector<boost::any> args) {
    if (func == "gen") {
        generateFunction(args);
        return;
    }
    else if (func == "setVar") {
        std::string funcName = PyEnvironment::Instance().runningFunc;
        setFunctionVar(funcName, args);
        return;
    }
}

void FunctionModule::evaluate(std::string func, PyObject &object) {

}

bool FunctionModule::varNameUsed(std::string varName) {
    std::string currentFunc = PyEnvironment::Instance().runningFunc;

    auto iter = pyFunctions.at(currentFunc)->localVars.find(varName);

    return !(iter == pyFunctions.at(currentFunc)->localVars.end());
}

void FunctionModule::generateFunction(std::vector<boost::any> funcStrings) {
    std::unique_ptr<PyFunction> function = pyFunctionBuilder.init()
            .parseFunctionSignature(boost::any_cast<std::string>(funcStrings[0]))
            .parseStatements(boost::any_cast<std::string>(funcStrings[1]))
            .build();

    pyFunctions.emplace(function->getName(), std::move(function));
}

void FunctionModule::setFunctionVar(const std::string &funcName, std::vector<boost::any> args) {
    auto vartype = boost::any_cast<PyConstants::VarTypes>(args[0]);
    const std::string varName = boost::any_cast<std::string>(args[1]);
    boost::any value = args[2];

    bool nameUsed = varNameUsed(varName);

    if (nameUsed && vartype != PyConstants::VarTypes::NONE) {
        if (pyFunctions.at(funcName)->localVars.at(varName)->type == vartype) {
            modifyFunctionVar(funcName, vartype, varName, value);
        } else {
            mutateFunctionVar(funcName, vartype, varName, value);
        }
    } else if (!nameUsed && vartype != PyConstants::VarTypes::NONE) {
        createFunctionVar(funcName, vartype, varName, value);
    }
}

void FunctionModule::mutateFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype,
                                       const std::string &varName, boost::any value) {
    switch ( vartype ) {
        case PyConstants::VarTypes::NUMBER: {
            pyFunctions.at(funcName)->localVars.at(varName) = std::make_shared<PyInt>(boost::any_cast<int>(value));
            break;
        }
        case PyConstants::VarTypes::BOOL: {
            pyFunctions.at(funcName)->localVars.at(varName) = std::make_shared<PyBool>(boost::any_cast<bool>(value));
            break;
        }
        case PyConstants::VarTypes::STRING: {
            pyFunctions.at(funcName)->localVars.at(varName) = std::make_shared<PyString>(boost::any_cast<std::string>(value));
            break;
        }
        default:
            break;
    }
}

void FunctionModule::modifyFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype,
                                       const std::string &varName, boost::any value) {
    switch ( vartype ) {
        case PyConstants::VarTypes::NUMBER: {
            pyFunctions.at(funcName)->localVars.at(varName)->setData<int>(boost::any_cast<int>(value));
            break;
        }
        case PyConstants::VarTypes::BOOL: {
            pyFunctions.at(funcName)->localVars.at(varName)->setData<bool>(boost::any_cast<bool>(value));
            break;
        }
        case PyConstants::VarTypes::STRING: {
            pyFunctions.at(funcName)->localVars.at(varName)->setData<std::string>(boost::any_cast<std::string>(value));
            break;
        }
        default:
            break;
    }
}

void FunctionModule::createFunctionVar(const std::string &funcName, PyConstants::VarTypes vartype,
                                       const std::string &varName, boost::any value) {
    switch ( vartype ) {
        case PyConstants::VarTypes::NUMBER: {
            pyFunctions.at(funcName)->localVars.insert(
                    std::make_pair(
                            varName,
                            std::make_shared<PyInt>(boost::any_cast<int>(value))
                    ));
            break;
        }
        case PyConstants::VarTypes::BOOL: {
            pyFunctions.at(funcName)->localVars.insert(
                    std::make_pair(
                            varName,
                            std::make_shared<PyBool>(boost::any_cast<bool>(value))
                    ));
            break;
        }
        case PyConstants::VarTypes::STRING: {
            pyFunctions.at(funcName)->localVars.insert(
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
