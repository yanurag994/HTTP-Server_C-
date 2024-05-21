#include <map>
#include <string>
#include <stdexcept>

std::map<std::string, std::string> arg_parser(int argc, char** argv) {
    std::map<std::string, std::string> args;

    if (argc % 2 == 0) {
        throw std::invalid_argument("Invalid number of arguments passed to the server: " + std::to_string(argc));
    }

    for (int i = 1; i < argc; i += 2) {
        std::string key = argv[i];
        if (key.size() < 2 || (key[0] != '-' || key[1] != '-')) {
            throw std::invalid_argument("Invalid argument key: " + key);
        }
        
        args.emplace(key.substr(2), argv[i + 1]);
    }

    return args;
}