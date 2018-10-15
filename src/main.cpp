#include "driver.hpp"

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

    Py::Driver driver;

    driver.parse_string(func, "func");

    return EXIT_SUCCESS;
}