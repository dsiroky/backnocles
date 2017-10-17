/// Tests entry.
///
/// @file

#include <string.h>

#include "backnocles/common/disablewarnings.hpp"
  #include <gtest/gtest.h>
#include "backnocles/common/enablewarnings.hpp"

//==========================================================================

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
