#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <string>
#include <vector>
#include <openssl/sha.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

class Conversion
{
    public:
        std::string sha256(const std::string inputStr);

        std::vector<size_t> getEncodedActions(const std::vector<std::string>& actionPatterns, decltype(actionPatterns)& currentActions);
};

#endif