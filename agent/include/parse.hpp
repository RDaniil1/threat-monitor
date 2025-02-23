#ifndef PARSE_HPP
#define PARSE_HPP

#include <vector>
#include <string>
#include <memory>
#include <array>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <algorithm>

#include "fmt/format.h"
#include "fmt/ranges.h"

class Parse
{
    private:
        std::string executeCommand(const char* cmd);
    
        std::vector<std::string> splitStringByNewline(const std::string& str);
    public:
        float getNetworkUsage(std::string networkInterface);
        std::string getMachineId();

        std::string getParseCommand(const std::vector<std::string>& actionPatterns);

        std::vector<std::string> getCurrentActions(const std::vector<std::string>& actionPatterns);
};

#endif