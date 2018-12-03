#include "py/pyenvironment.hpp"

int main(int argc, char** argv) {

    if (argc >= 2) {
        PyEnvironment::Instance().parseFile(argv[1]);

        std::cout << "Mutated Variable: ";

        for (auto& string : PyEnvironment::Instance().mutatedVars) {
            std::cout << string + ", ";
        }

        std::cout << std::endl;

        std::cout << "Nested if/else level: ";

        for (auto level : PyEnvironment::Instance().nestedIfCount) {
            std::cout << level << " level, ";
        }

        std::cout << std::endl;

        std::cout << "Recursive function ends: ";

        for (auto& pair : PyEnvironment::Instance().recursiveFunctionsEnd) {
            if (pair.second) {
                std::cout << "Yes, ";
            }
            else {
                std::cout << "No, ";
            }
        }

        std::cout << std::endl;
    }
    else {
        std::string input;
        while (true) {
            std::cout << ">>> ";
            std::cin >> input;
            if (input == "exit()") {
                break;
            }

            PyEnvironment::Instance().parseStatement(input);
        }
    }

    return EXIT_SUCCESS;
}