#ifndef MYPYTHON_STDPRINT_HPP
#define MYPYTHON_STDPRINT_HPP

#include "../pyfunction.hpp"

class StdPrint : public PyFunction {
public:
    StdPrint() : PyFunction() {
        funcName = "print";
        isStdFunc = true;
        eval = [=](std::vector<std::string> args){ StdPrint::evaluate(args); };
    }

protected:
    void evaluate(std::vector<std::string> args) override;
};

#endif //MYPYTHON_STDPRINT_HPP
