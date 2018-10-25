#include "py/pyenvironment.hpp"

class foo {
public:
    explicit foo(int x) : test(x) {}
    int test;
};

int main(int argc, char** argv) {

    if (argc == 2) {
        PyEnvironment::Instance().parseFile(argv[1]);
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