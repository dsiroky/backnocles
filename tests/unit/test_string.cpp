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
  EXPECT_EQ(backnocles::trim_view(""), ""sv);
  EXPECT_EQ(backnocles::trim_view("abc"), "abc"sv);
  EXPECT_EQ(backnocles::trim_view("  \t"), ""sv);
  EXPECT_EQ(backnocles::trim_view(" \tqwe"), "qwe"sv);
  EXPECT_EQ(backnocles::trim_view("sdf \t"), "sdf"sv);
  EXPECT_EQ(backnocles::trim_view("\t   oioi \t"), "oioi"sv);
}

//==========================================================================
} // namespace
