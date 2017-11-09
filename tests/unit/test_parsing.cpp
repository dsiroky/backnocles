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

template<typename T>
struct CommandFactory;

//==========================================================================

/// Command type is "int".
template<>
struct CommandFactory<int>
{
  template<typename Iterator>
  static auto create(Iterator, Iterator)
  {
    return int{0};
  }
};

TEST(ProcessTokens, EmptyTokens_ThrowsException)
{
  const auto call_map = backnocles::make_type_call_map(
                            backnocles::TypeCallMapItem<int>{"dummy"}
                          );
  auto processor = [&](const int&){};

  EXPECT_THROW(backnocles::Tokens<CommandFactory>::process(
                                  call_map,
                                  processor,
                                  {}),
                backnocles::TypeCallMapNotFound);
}

//--------------------------------------------------------------------------

/// a command type
struct cmd_quit_t {};

template<>
struct CommandFactory<cmd_quit_t>
{
  template<class Iterator>
  static auto create(Iterator, Iterator)
  {
    return cmd_quit_t{};
  }
};

TEST(ProcessTokens, PassQuitToken_CallsQuitCmdOverload)
{
  // call flag
  bool called{false};

  const auto call_map = backnocles::make_type_call_map(
                            backnocles::TypeCallMapItem<cmd_quit_t>{"quit"}
                          );
  auto processor = [&](const cmd_quit_t&){ called = true; };

  backnocles::Tokens<CommandFactory>::process(call_map, processor, {"quit"});

  EXPECT_TRUE(called);
}

//--------------------------------------------------------------------------

/// @defgroup commandtypes command types
/// @{

struct cmd_without_params_t {};

struct cmd_with_params_t
{
  std::string p1;
  int p2{0};
  bool p3{false};
};

/// @}

template<>
struct CommandFactory<cmd_without_params_t>
{
  template<class Iterator>
  static auto create(Iterator, Iterator)
  {
    return cmd_without_params_t{};
  }
};

template<>
struct CommandFactory<cmd_with_params_t>
{
  template<class Iterator>
  static auto create(Iterator it, Iterator)
  {
    return cmd_with_params_t{
                            std::string{*it++},
                            std::stoi(std::string{*it++}),
                            *it == "true"sv
                          };
  }
};


TEST(ProcessTokens, PassMultipleTokens_CallsCorrectOverloadWithParameters)
{
  // call flags
  bool called_without_params{false};
  bool called_with_params{false};
  cmd_with_params_t created_cmd{};

  const auto call_map = backnocles::make_type_call_map(
          backnocles::TypeCallMapItem<cmd_without_params_t>{"cmdwithout"},
          backnocles::TypeCallMapItem<cmd_with_params_t>{"cmdwith"}
        );
  auto processor = backnocles::make_overload(
                [&](const cmd_without_params_t&)
                {
                    called_without_params = true;
                },
                [&](const cmd_with_params_t& cmd)
                {
                    called_with_params = true;
                    created_cmd = cmd;
                }
            );

  backnocles::Tokens<CommandFactory>::process(call_map, processor,
                              {"cmdwith", "abc", "837", "true"});

  EXPECT_FALSE(called_without_params);
  EXPECT_TRUE(called_with_params);
  EXPECT_EQ(created_cmd.p1, "abc"sv);
  EXPECT_EQ(created_cmd.p2, 837);
  EXPECT_TRUE(created_cmd.p3);
}

//--------------------------------------------------------------------------

//TEST(ProcessTokens, ExceptionFromCommandConstructionPropagatesOut)
//{
//  // TODO
//}
//
////--------------------------------------------------------------------------
//
//TEST(ProcessTokens, ThrowExceptionOnUnknownCommand)
//{
//  // TODO
//}

//==========================================================================
} // namespace
