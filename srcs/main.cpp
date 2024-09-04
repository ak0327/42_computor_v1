#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "[Error] invalid argument.\n"
                     "        Expected: $> ./computor <equation>" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "arg: [" << argv[1] << "]" << std::endl;
    return EXIT_SUCCESS;
}
