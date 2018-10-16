#include <sstream>

#include "py/pyenvironment.hpp"

int main(int argc, char** argv) {

    /*PyFunction pyFunction("print", "(\"BIG,- HATE\", \"woopty\", scoop38, 0)");
    pyFunction.parseFunctionHeader();*/

    std::string func = "def help_6545(pie, cake):\n"
                       "   asdsadasd = asdassds\n"
                       "   asdadsadasdsasadds\n"
                       "   sfsdfs = dfdsfdsf + dsfdsf\n"
                       "\n"
                       "   asdsdsdsdadsdasddasdsadsad\n"
                       "   if something > that:\n"
                       "      eat big\n"
                       "      big big\n"
                       "      if big != big:\n"
                       "         ahahahaha\n"
                       "\n"
                       "   asdsdaddasd\n"
                       "   if other > that:\n"
                       "      asd\n"
                       "\n";

    /*PyFunctionBuilder pyFunctionBuilder;
    std::unique_ptr<PyFunction> pyFunction = pyFunctionBuilder.init(func);
    std::cout << pyFunction->getName() << std::endl;*/

    /*std::string ifblock = "   if something > that:\n"
                          "      eat big\n"
                          "      big big";

    boost::trim(ifblock);
    boost::regex ifregex{R"(^if)"};
    boost::smatch s;

    if(boost::regex_search(ifblock, s, ifregex)) {
        std::cout << ifblock << std::endl;
    }*/

    std::string defStr = "def test():\n"
                           "   x = \"ye\"\n"
                           "   print(x)\n"
                           "\n"
                           "test()";

    std::string printStr = "x = 1\n"
                           "if x != 1:\n"
                           "   x = 2\n"
                           "print(\"x = \",x)";

    PyEnvironment::Instance().parseStatement(printStr);

    return EXIT_SUCCESS;
}