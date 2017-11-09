/// User input tokenizing, parsing.
///
/// @file

#pragma once
#ifndef PARSING_HPP__K6HTWFVC
#define PARSING_HPP__K6HTWFVC

#include <string_view>
#include <type_traits>

#include "backnocles/utils/functional.hpp"

//==========================================================================
namespace backnocles {
//==========================================================================

/// Converts e.g. "cmd param1 param2" to individual words. Tokens are divided
/// by a single space.
/// @param input user text input
/// @param first iterator which will be used to store the tokens
template<class ForwardIt>
inline void tokenize(const std::string_view input, ForwardIt first)
{
  if (input.size() == 0)
  {
    return;
  }

  size_t space_pos{0};
  size_t new_start{0};
  while (true)
  {
    space_pos = input.find(' ', new_start);
    if (space_pos == std::string_view::npos)
    {
      *first = input.substr(new_start);
      break;
    }
    *first = input.substr(new_start, space_pos - new_start);
    ++first;
    new_start = space_pos + 1;
  }
}

//==========================================================================

/// a "hiding" namespace
namespace detail::parsing {

/// A wrapper that will use the Factory to produce a particular value and
/// passes it to the processor.
template<class Iterator, template<typename> typename Factory, class Processor>
class ProcessorWrap
{
public:
  ProcessorWrap(Iterator first, Iterator last, Processor& p)
    : m_first{first}, m_last{last}, m_processor{p}
  {}

  /// `call<T>() -> Factory<T>::create() -> processor(T)`
  template<typename T>
  void call() const
  {
    // user might accidentally return a different type other then the
    // factory template
    static_assert(
          std::is_same_v<decltype(Factory<T>::create(m_first, m_last)),
                          T>
        );

    m_processor(Factory<T>::create(m_first, m_last));
  }

private:
  Iterator m_first, m_last;
  Processor& m_processor;
};

} // namespace

//--------------------------------------------------------------------------

/// This class is a "partial specialization" for the Tokens<Factory>::process()
/// @param Factory a template template where each command has its own specialization.
///                 For examples see tests::CommandFactory. Each specialization
///                 must have a static method `call()` with iterator arguments
///                 `first` and `last` and a return type must be equivalent to
///                 the Factory specialization.
template<template<typename> typename Factory>
struct Tokens
{
  /// @param call_map see make_type_call_map()
  /// @param processor an instance with overloaded `void operator()(const T&)`
  ///                   methods which will be created with values produced by
  ///                   the factory.
  /// @param tokens a list of tokens where the first one is a key to the call
  ///               map and the rest are raw values for the factory.
  template<class CallMap, class Processor>
  static void process(CallMap& call_map,
                        Processor& processor,
                        std::initializer_list<std::string_view> tokens)
  {
    if (tokens.size() == 0)
    {
      throw TypeCallMapNotFound{};
    }
    const std::string_view command_name{*tokens.begin()};
    using ProcessorWrap = detail::parsing::ProcessorWrap<
                                                decltype(begin(tokens)),
                                                Factory,
                                                Processor
                                              >;
    call_map.call(command_name,
                  ProcessorWrap{std::next(begin(tokens)), end(tokens), processor});
  }
};


//==========================================================================
} // namespace

#endif /* include guard */
