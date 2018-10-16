#ifndef MYPYTHON_STDPRINT_HPP
#define MYPYTHON_STDPRINT_HPP

#include "../pyfunction.hpp"

class StdPrint : public PyFunction {
public:
    StdPrint() { funcName = "print"; }
    void evaluate(std::vector<std::string> args) override;
};

#endif //MYPYTHON_STDPRINT_HPP
