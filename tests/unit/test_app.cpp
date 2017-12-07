/// Test main app functions.
///
/// @file

#include <string>

#include "backnocles/utils/disablewarnings.hpp"
  #include <gtest/gtest.h>
  #include <gmock/gmock.h>
#include "backnocles/utils/enablewarnings.hpp"

#include "backnocles/app.hpp"

//==========================================================================
namespace tests {
//==========================================================================

TEST(App, Request_UnknownCommand_OutputsUnknownCommand)
{
  backnocles::App app{};

  std::string output{};
  bool quit_flag{false};

  app.request("ndksfsldfjh",
              [&](const std::string_view& output_){ output = output_; },
              [&](){ quit_flag = true; });

  EXPECT_EQ(output, "unknown command: ndksfsldfjh");
  EXPECT_FALSE(quit_flag);
}

//--------------------------------------------------------------------------

TEST(App, Request_Quit_OutputsByeAndCallsQuit)
{
  backnocles::App app{};

  std::string output{};
  bool quit_flag{false};

  app.request("quit",
              [&](const std::string_view& output_){ output = output_; },
              [&](){ quit_flag = true; });

  EXPECT_EQ(output, "bye");
  EXPECT_TRUE(quit_flag);
}

//--------------------------------------------------------------------------

TEST(App, Request_AddHippo_OutputsAddedAndDoesNotQuit)
{
  backnocles::App app{};

  std::string output{};
  bool quit_flag{false};

  app.request("addanimal hippo",
              [&](const std::string_view& output_){ output = output_; },
              [&](){ quit_flag = true; });

  EXPECT_EQ(output, "hippo id:1 added");
  EXPECT_FALSE(quit_flag);
}

//--------------------------------------------------------------------------

TEST(App, Request_AddMultipleAnimals_OutputsAddedWithDifferentIdsAndDoesNotQuit)
{
  backnocles::App app{};

  {
    std::string output{};
    bool quit_flag{false};

    app.request("addanimal hippo",
                [&](const std::string_view& output_){ output = output_; },
                [&](){ quit_flag = true; });

    EXPECT_EQ(output, "hippo id:1 added");
    EXPECT_FALSE(quit_flag);
  }
  {
    std::string output{};
    bool quit_flag{false};

    app.request("addanimal turtle",
                [&](const std::string_view& output_){ output = output_; },
                [&](){ quit_flag = true; });

    EXPECT_EQ(output, "turtle id:2 added");
    EXPECT_FALSE(quit_flag);
  }
}

//==========================================================================
} // namespace
