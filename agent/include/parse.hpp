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
#include <filesystem>

#include "fmt/format.h"
#include "fmt/ranges.h"

/**
* @brief Parse data base on patterns to prepare it for neural network.
*/
class Parse
{
    public:
        /**
        * @brief Get network usage from host based on rx/tx bytes.
        *
        * @param networkInterface name of network interface
        * @return value, based on rx/tx bytes from host
        */
        float getNetworkUsage(std::string networkInterface);
        /**
        * @brief Get machine-id from host for using in database as unique data.
        *
        * @return host machine-id  
        */
        std::string getMachineId();
        /**
        * @brief Create shell command for searching events in journalctl.
        *
        * @param actionPatterns event patterns from database
        * @return shell command for getting current user events 
        */
        std::string getParseCommand(const std::vector<std::string>& actionPatterns);
        /**
        * @brief Get current user actions based on patterns.
        *
        * @param actionPatterns event patterns from database
        * @return vector with finded user's events 
        */
        std::vector<std::string> getCurrentActions(const std::vector<std::string>& actionPatterns);
    private:
        /**
        * @brief Execute any command in shell.
        *
        * @param cmd command for execution
        * @return command output from stdout 
        */
        std::string executeCommand(const char* cmd);
        /**
        * @brief Split single string in multiple strings by newline.
        *
        * @param str input string 
        * @return command output from stdout 
        */
        std::vector<std::string> splitStringByNewline(const std::string& str);
};

#endif // PARSE_HPP