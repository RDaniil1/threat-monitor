#include "conversion.hpp"
#include "parse.hpp"

#include "dotenv.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(ConversionTest, HashCheck) 
{
    Conversion conversion;
    std::string expectedResultSha256 = "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"; 
    std::string realOutput = conversion.sha256("test");

    EXPECT_STREQ(realOutput.c_str(), expectedResultSha256.c_str());
}

TEST(ConversionTest, PatternsCheck)
{
    Conversion conversion;
    std::vector<std::string> actionPatterns = {"device found", "sudoers"};
    std::vector<std::string> currentActions = {"USB device found", "user not in sudoers"};

    auto encodedActions = conversion.getEncodedActions(actionPatterns, currentActions);
    ASSERT_THAT(encodedActions, ::testing::ElementsAreArray({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 })) << "Actions from database and founded ones are not equal";
}

TEST(ParseTest, BasicFunctionalityCheck)
{
    Parse parse;
    auto networkUsage = parse.getNetworkUsage("wlan0");
    ASSERT_THAT(networkUsage, ::testing::Ge(0.0f)) << "Network usage can't be negative";

    auto machineId = parse.getMachineId();
    ASSERT_THAT(machineId, ::testing::MatchesRegex("^[a-z0-9]{32}$")) << "Result is not equal to machineId pattern";

    std::vector<std::string> actionPatterns = {"device found", "sudoers"};
    auto parseCommand = parse.getParseCommand(actionPatterns);
    EXPECT_STREQ(parseCommand.c_str(), "journalctl -q -b 0 -S \"1 minute ago\" -n 10 -g 'device found|sudoers'") 
        << "Command for searching malicious events is incorrect";
    
    std::vector<std::string> currentActions = parse.getCurrentActions(actionPatterns);
    EXPECT_THAT(currentActions, ::testing::Conditional(currentActions.empty(), 
                                                       ::testing::IsEmpty(), 
                                                       ::testing::ElementsAreArray({"USB device found", "user not in sudoers"}))
                                                       )
        << "Expected current actions based on patterns from database";
}

TEST(ParseTest, EmptyCommandInputCheck)
{
    Parse parse;
    auto emptyVectorParseCommand = parse.getParseCommand({});
    auto emptyStringParseCommand = parse.getParseCommand({""});
    EXPECT_STREQ(emptyVectorParseCommand.c_str(), "") << "After empty vector expected empty string";
    EXPECT_STREQ(emptyStringParseCommand.c_str(), "") << "After empty string expected similar responce";
}

TEST(ParseTest, EmptyCurrentActionsCheck)
{
    Parse parse;
    auto emptyCurrentActions = parse.getCurrentActions({});
    ASSERT_TRUE(emptyCurrentActions.empty()) << "Expected empty empty current actions";
}

