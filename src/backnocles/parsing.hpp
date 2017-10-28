/// User input tokenizing, parsing.
///
/// @file

#pragma once
#ifndef PARSING_HPP__K6HTWFVC
#define PARSING_HPP__K6HTWFVC

#include <string_view>
#include <type_traits>

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
} // namespace

#endif /* include guard */
