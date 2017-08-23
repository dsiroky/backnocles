/// Tests entry.
///
/// @file

#include <string.h>

#include "downloader/disablewarnings.hpp"
  #include <gtest/gtest.h>
#include "downloader/enablewarnings.hpp"

//==========================================================================

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
