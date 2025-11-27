#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include <openssl/sha.h>

/**
* @brief Converts actions from journalctl to indexes. 
*/
class Conversion
{
    public:
        /**
        * @brief Calculate hash from input data for machine-id.
        *
        * @param inputStr input string for getting hash
        * @return hash value from input string
        */
        std::string sha256(const std::string inputStr);
        /**
        * @brief Get encoded actions for sending to database and work with neural network.
        *
        * @param actionPatterns available action patterns from database that used for searching malicious activity
        * @param currentActions current actions, ecxtracted from journalctl
        * @return encoded actions, which based on currentActions 
        */
        std::vector<size_t> getEncodedActions(const std::vector<std::string>& actionPatterns, decltype(actionPatterns)& currentActions);
};

#endif // CONVERSION_HPP