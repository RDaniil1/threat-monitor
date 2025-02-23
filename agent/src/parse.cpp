#include "parse.hpp"

std::string Parse::executeCommand(const char* cmd) 
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::vector<std::string> Parse::splitStringByNewline(const std::string& str)
{
    std::vector<std::string> result;
    std::stringstream ss(str);

    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);

    return result;
}

float Parse::getNetworkUsage(std::string networkInterface)
{
    std::string recCommand = fmt::format("cat /sys/class/net/{}/statistics/rx_bytes", networkInterface);
    std::string tranCommand = fmt::format("cat /sys/class/net/{}/statistics/tx_bytes", networkInterface);

    std::string recBefore = executeCommand(recCommand.c_str());
    std::string tranBefore = executeCommand(tranCommand.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::string recAfter = executeCommand(recCommand.c_str());
    std::string tranAfter = executeCommand(tranCommand.c_str());

    float r1 = std::stof(recBefore);
    float t1 = std::stof(tranBefore);
    float r2 = std::stof(recAfter);
    float t2 = std::stof(tranAfter);

    return std::fabs((r2 + t2 - r1 - t1) / 1024);
}

std::string Parse::getMachineId()
{
    std::string machineId = executeCommand("cat /etc/machine-id");
    machineId.pop_back();
    return machineId;
}

std::string Parse::getParseCommand(const std::vector<std::string>& actionPatterns)
{
    if (actionPatterns.empty() || actionPatterns[0] == "")
    {
        return "";
    }

    std::string parseCommand = fmt::format("journalctl -q -b 0 -S \"1 minute ago\" -n 10 -g '{}'", 
                                            fmt::join(actionPatterns, "|"));
    return parseCommand;
}

std::vector<std::string> Parse::getCurrentActions(const std::vector<std::string>& actionPatterns) 
{
    std::vector<std::string> currentActions;

    std::string parseCommand = getParseCommand(actionPatterns);
    std::string journalctlRes = executeCommand(parseCommand.c_str());

    currentActions = splitStringByNewline(journalctlRes);
    if (currentActions.empty())
    {
        return {};
    }
    
    std::reverse(currentActions.begin(), currentActions.end());
    currentActions.pop_back();

    return currentActions;
}