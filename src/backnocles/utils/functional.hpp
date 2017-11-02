/// Functional stuff.
///
/// @file

#pragma once
#ifndef FUNCTIONAL_HPP__KG9RFBRT
#define FUNCTIONAL_HPP__KG9RFBRT

#include <utility>

//==========================================================================
namespace backnocles {
//==========================================================================

/// Inherit from given callables to provide an aggregated operator().
template<class... Funcs>
struct overload : Funcs...
{
  constexpr overload(Funcs&&... fs)
    : Funcs{std::forward<Funcs>(fs)}...
  {}
};

/// Overload maker.
template<class... Funcs>
constexpr auto make_overload(Funcs&&... fs)
{
  return overload<Funcs...>{std::forward<Funcs>(fs)...};
}

//==========================================================================
} // namespace

#endif /* include guard */
