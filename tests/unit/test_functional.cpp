/// Test functional stuff.
///
/// @file

#include <functional>

#include <gtest/gtest.h>

#include "backnocles/utils/functional.hpp"

//==========================================================================
namespace tests {
//==========================================================================

void dummy_doubleptr_signature_func(double*) {}

TEST(Overload, CallsCorrectSignature1)
{
  bool int_called{false};
  int int_value{0};
  bool char_called{false};
  bool floatptr_called{false};

  auto f = backnocles::make_overload(
                [&](int v) { int_called = true; int_value = v; },
                [&](char) { char_called = true; },
                [&](float*) { floatptr_called = true; },
                std::function<decltype(dummy_doubleptr_signature_func)>{
                                                  dummy_doubleptr_signature_func}
              );

  f(int{3});

  EXPECT_TRUE(int_called);
  EXPECT_EQ(int_value, 3);
  EXPECT_FALSE(char_called);
  EXPECT_FALSE(floatptr_called);
}

//--------------------------------------------------------------------------

TEST(Overload, CallsCorrectSignature2AndReturnsCorrectValue)
{
  bool int_called{false};
  bool char_called{false};
  char char_value{0};
  bool floatptr_called{false};

  auto f = backnocles::make_overload(
                [&](int) { int_called = true; return 10; },
                [&](char v) { char_called = true; char_value = v; return 11; },
                [&](float*) { floatptr_called = true; return 12; }
              );

  const auto ret = f(char{5});

  EXPECT_FALSE(int_called);
  EXPECT_TRUE(char_called);
  EXPECT_EQ(char_value, 5);
  EXPECT_FALSE(floatptr_called);
  EXPECT_EQ(ret, 11);
}

//--------------------------------------------------------------------------

TEST(Overload, ConstInstance_CallsCorrectSignature)
{
  bool int_called{false};
  bool char_called{false};
  bool floatptr_called{false};
  float dummy_val{0};
  float* floatptr_value{nullptr};

  const auto f = backnocles::make_overload(
                    [&](int) { int_called = true; },
                    [&](char) { char_called = true; },
                    [&](float* v) { floatptr_called = true; floatptr_value = v; }
                  );

  f(&dummy_val);

  EXPECT_FALSE(int_called);
  EXPECT_FALSE(char_called);
  EXPECT_TRUE(floatptr_called);
  EXPECT_EQ(floatptr_value, &dummy_val);
}

//==========================================================================
} // namespace
