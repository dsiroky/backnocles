/// Tests entry.
///
/// @file

#include <string.h>

#include "backnocles/utils/disablewarnings.hpp"
  #include <gtest/gtest.h>
#include "backnocles/utils/enablewarnings.hpp"

//==========================================================================

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
