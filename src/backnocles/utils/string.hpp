/// String utilities.
///
/// @file

#pragma once
#ifndef STRING_HPP__WNF4LEYQ
#define STRING_HPP__WNF4LEYQ

#include <cassert>
#include <string_view>
#include <iostream>

#include "backnocles/utils/disablewarnings.hpp"
  #include <boost/algorithm/string/trim.hpp>
#include "backnocles/utils/enablewarnings.hpp"

//==========================================================================
namespace bknc {
//==========================================================================

/// Trim whitespaces on both ends.
/// @return subset of the string_view
inline std::string_view trim(const std::string_view s)
{
  const auto match_non_whitespace = [](const char c)
  {
    return (c != ' ') && (c != '\t');
  };

  const auto end_it = std::find_if(s.rbegin(), s.rend(), match_non_whitespace);
  if (end_it == s.rend())
  {
    return {s.data(), 0};
  }

  const auto start_it = std::find_if(s.begin(), s.end(), match_non_whitespace);

  const auto n = &*end_it - start_it + 1;
  assert(n > 0);
  return {start_it, static_cast<size_t>(n)};
}

//==========================================================================
} // namespace

#endif /* include guard */
