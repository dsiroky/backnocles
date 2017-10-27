/// String utilities.
///
/// @file

#pragma once
#ifndef STRING_HPP__WNF4LEYQ
#define STRING_HPP__WNF4LEYQ

#include <cassert>
#include <string_view>
#include <string>
#include <iostream>

#include "backnocles/utils/disablewarnings.hpp"
  #include <boost/algorithm/string/trim.hpp>
#include "backnocles/utils/enablewarnings.hpp"

//==========================================================================
namespace backnocles {
//==========================================================================

/// Fast trim whitespaces on both ends without any heap allocations.
inline std::string_view trim_view(const std::string_view s)
{
  // this is a wheel reinvention, boost::trim_copy() might be used

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

  const auto n = &*end_it - &*start_it + 1;
  assert(n > 0);
  return std::string_view{&*start_it, static_cast<size_t>(n)};
}

//--------------------------------------------------------------------------

/// Safer version of trim_view().
inline std::string trim(const std::string_view s)
{
  return std::string{trim_view(s)};
}

//==========================================================================
} // namespace

#endif /* include guard */
