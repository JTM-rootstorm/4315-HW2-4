#include <utility>

//
// Created by mgjus on 10/15/2018.
//

#ifndef MYPYTHON_PYSTATEMENT_HPP
#define MYPYTHON_PYSTATEMENT_HPP

#include <string>

class PyStatement {
public:
    PyStatement() = default;
    explicit PyStatement(std::string _statement) : statement(std::move(_statement)) {};

    virtual void evaluate();

protected:
    std::string statement;
};

#endif //MYPYTHON_PYSTATEMENT_HPP
