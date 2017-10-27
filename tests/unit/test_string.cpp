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
  EXPECT_EQ(backnocles::trim(""), ""sv);
  EXPECT_EQ(backnocles::trim("  \t"), ""sv);
  EXPECT_EQ(backnocles::trim("abc"), "abc"sv);
  EXPECT_EQ(backnocles::trim(" \tqwe"), "qwe"sv);
  EXPECT_EQ(backnocles::trim("sdf \t"), "sdf"sv);
  EXPECT_EQ(backnocles::trim("\t   oioi \t"), "oioi"sv);
}

//==========================================================================
} // namespace
