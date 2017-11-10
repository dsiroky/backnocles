/// Test functional stuff.
///
/// @file

#include <functional>

#include "backnocles/utils/disablewarnings.hpp"
  #include <gtest/gtest.h>
  #include <gmock/gmock.h>
#include "backnocles/utils/enablewarnings.hpp"

#include "backnocles/utils/functional.hpp"

//==========================================================================
namespace tests {
//==========================================================================

using namespace std::string_view_literals;

//==========================================================================

TEST(Overload, SingleLambda)
{
  bool int_called{false};
  int int_value{0};

  auto f = backnocles::make_overload(
                [&](int v) { int_called = true; int_value = v; }
              );

  f(int{9});

  EXPECT_TRUE(int_called);
  EXPECT_EQ(int_value, 9);
}

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

struct SpecializedCallee
{
  MOCK_METHOD0(call_int, void());
  MOCK_METHOD0(call_float, void());
  MOCK_METHOD0(call_charptr, void());

  template<typename T>
  void call();
};
template<>
void SpecializedCallee::call<int>() { call_int(); }
template<>
void SpecializedCallee::call<float>() { call_float(); }
template<>
void SpecializedCallee::call<char*>() { call_charptr(); }

//--------------------------------------------------------------------------

TEST(TypeCallMap, Call)
{
  constexpr auto m = backnocles::make_type_call_map(
                        backnocles::TypeCallMapItem<int>{"aaaint"sv},
                        backnocles::TypeCallMapItem<char*>{"chpt"sv},
                        backnocles::TypeCallMapItem<float>{"bbbfloat"sv}
                      );

  {
    SpecializedCallee callee;
    EXPECT_CALL(callee, call_int()).Times(0);
    EXPECT_CALL(callee, call_charptr()).Times(0);
    EXPECT_CALL(callee, call_float()).Times(1);

    m.call("bbbfloat", callee);
  }

  {
    SpecializedCallee callee;
    EXPECT_CALL(callee, call_int()).Times(1);
    EXPECT_CALL(callee, call_charptr()).Times(0);
    EXPECT_CALL(callee, call_float()).Times(0);

    m.call("aaaint", callee);
  }
}

//--------------------------------------------------------------------------

TEST(TypeCallMap, Call_UnknownTag_ThrowsException)
{
  constexpr auto m = backnocles::make_type_call_map(
                        backnocles::TypeCallMapItem<int>{"xxxint"sv},
                        backnocles::TypeCallMapItem<float>{"yyyfloat"sv}
                      );

  SpecializedCallee callee;
  EXPECT_CALL(callee, call_int()).Times(0);
  EXPECT_CALL(callee, call_charptr()).Times(0);
  EXPECT_CALL(callee, call_float()).Times(0);

  EXPECT_THROW(m.call("othertag", callee), backnocles::TypeCallMapNotFound);
}

//--------------------------------------------------------------------------

TEST(TypeCallMap, Call_EmptyList_ThrowsException)
{
  constexpr auto m = backnocles::make_type_call_map();

  SpecializedCallee callee;
  EXPECT_CALL(callee, call_int()).Times(0);
  EXPECT_CALL(callee, call_charptr()).Times(0);
  EXPECT_CALL(callee, call_float()).Times(0);

  EXPECT_THROW(m.call("othertag", callee), backnocles::TypeCallMapNotFound);
}

//--------------------------------------------------------------------------

TEST(TypeCallMap, Call_UnknownTag_CallCustomCallback)
{
  constexpr auto m = backnocles::make_type_call_map(
                        backnocles::TypeCallMapItem<int>{"xxxint"sv},
                        backnocles::TypeCallMapItem<float>{"yyyfloat"sv}
                      );

  SpecializedCallee callee;
  EXPECT_CALL(callee, call_int()).Times(0);
  EXPECT_CALL(callee, call_charptr()).Times(0);
  EXPECT_CALL(callee, call_float()).Times(0);

  std::string unknown_tag;
  m.call("othertag", callee,
          [&](const std::string_view tag)
          {
            unknown_tag = tag;
          });
  EXPECT_EQ(unknown_tag, "othertag");
}

//==========================================================================
} // namespace
