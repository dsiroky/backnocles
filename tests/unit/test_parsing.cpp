/// Test user input parsing.
///
/// @file

#include <vector>
#include <string_view>

#include "backnocles/utils/disablewarnings.hpp"
  #include <gtest/gtest.h>
#include "backnocles/utils/enablewarnings.hpp"

#include "backnocles/parsing.hpp"

//==========================================================================
namespace tests {
//==========================================================================

using namespace std::string_view_literals;

//==========================================================================

TEST(Tokenize, EmptyString_NoItems)
{
  std::vector<std::string_view> tokens;

  backnocles::tokenize("", std::back_inserter(tokens));

  EXPECT_EQ(tokens.size(), 0);
}

//--------------------------------------------------------------------------

TEST(Tokenize, SingleToken_SingleItem)
{
  std::vector<std::string_view> tokens;

  backnocles::tokenize("somecommand", std::back_inserter(tokens));

  ASSERT_EQ(tokens.size(), 1);
  EXPECT_EQ(tokens[0], "somecommand"sv);
}

//--------------------------------------------------------------------------

TEST(Tokenize, MultipleTokens_EquivalentItems)
{
  std::vector<std::string_view> tokens;

  backnocles::tokenize("othercommand param1 param2 param3",
                        std::back_inserter(tokens));

  ASSERT_EQ(tokens.size(), 4);
  EXPECT_EQ(tokens[0], "othercommand"sv);
  EXPECT_EQ(tokens[1], "param1"sv);
  EXPECT_EQ(tokens[2], "param2"sv);
  EXPECT_EQ(tokens[3], "param3"sv);
}

//==========================================================================
} // namespace
