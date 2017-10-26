/// Test string utilities.
///
/// @file

#include <string>

#include <gtest/gtest.h>

#include "backnocles/utils/string.hpp"

//==========================================================================
namespace tests {
//==========================================================================

using namespace std::string_view_literals;

//--------------------------------------------------------------------------

TEST(String, Trim)
{
  EXPECT_EQ(bknc::trim(""), ""sv);
  EXPECT_EQ(bknc::trim("  \t"), ""sv);
  EXPECT_EQ(bknc::trim("abc"), "abc"sv);
  EXPECT_EQ(bknc::trim(" \tqwe"), "qwe"sv);
  EXPECT_EQ(bknc::trim("sdf \t"), "sdf"sv);
  EXPECT_EQ(bknc::trim("\t   oioi \t"), "oioi"sv);
}

//==========================================================================
} // namespace
