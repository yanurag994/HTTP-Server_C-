#include <fstream>
#include <regex>

#include <sys/socket.h>

class Backend {
    std::string directory;
public:
    Backend() {};
    Backend(const std::string& p_directory) : directory(p_directory) {};
    void handle_request(int client);
};

struct GET {};

struct POST {};