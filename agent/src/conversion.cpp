/*********************************************************************
 * @file  conversion.cpp
 * 
 * @brief Implementation of the class Conversion.
 *********************************************************************/
#include "conversion.hpp"

std::string Conversion::sha256(const std::string inputStr)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    auto data = (const unsigned char*)inputStr.c_str();
    SHA256(data, inputStr.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::vector<size_t> Conversion::getEncodedActions(const std::vector<std::string>& actionPatterns, decltype(actionPatterns)& currentActions)
{
    std::vector<size_t> encodedActions;
    encodedActions.reserve(currentActions.size());

    for (const auto& action : currentActions)
    {
        for (size_t i{}; i < actionPatterns.size(); ++i)
        {
            std::string pattern = actionPatterns[i];
            if (action.find(pattern) != std::string::npos) 
            {
                encodedActions.push_back(i);
                continue;
            }
        }
    }

    if (encodedActions.size() < 10)
    {
        size_t reservedEmtptyActions = 10 - encodedActions.size();
        std::vector<size_t> reservedOnes(reservedEmtptyActions);
        encodedActions.insert(encodedActions.begin(), reservedOnes.begin(), reservedOnes.end());
        return encodedActions;
    }

    return encodedActions;
}


