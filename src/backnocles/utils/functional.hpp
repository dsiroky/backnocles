/// Functional stuff.
///
/// @file

#pragma once
#ifndef FUNCTIONAL_HPP__KG9RFBRT
#define FUNCTIONAL_HPP__KG9RFBRT

#include <utility>
#include <string_view>
#include <stdexcept>

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

//--------------------------------------------------------------------------

/// Overload maker.
template<class... Funcs>
constexpr auto make_overload(Funcs&&... fs)
{
  return overload<Funcs...>{std::forward<Funcs>(fs)...};
}

//==========================================================================
/// @defgroup typecallmap Type call map
/// @{
/// Example:
/// @code
/// struct MyCallee
/// {
///   void call<int>() { std::cout << "int" << '\n'; }
///   void call<float>() { std::cout << "float" << '\n'; }
///   void call<char*>() { std::cout << "char*" << '\n'; }
/// };
///
/// constexpr auto tcm = backnocles::make_type_call_map(
///                              backnocles::TypeCallMapItem<int>{"myint"},
///                              backnocles::TypeCallMapItem<char*>{"someptr"},
///                              backnocles::TypeCallMapItem<float>{"yourfloat"}
///                           );
///
/// tcm.call("someptr", callee);
/// @endcode
//==========================================================================

/// Exception thrown when a call is made with an unknown tag.
struct TypeCallMapNotFound : std::exception {};

/// Item of the type call map.
template<typename T>
struct TypeCallMapItem
{
  constexpr TypeCallMapItem(const std::string_view tag_)
    : tag{tag_}
  {}

  const std::string_view tag;
};

//--------------------------------------------------------------------------
namespace detail {
//--------------------------------------------------------------------------

template<class NextType, class T>
class TCMNode
{
public:
  constexpr TCMNode(NextType next, const std::string_view tag)
    : m_next{next}, m_tag{tag}
  {
  }

  template<class Callee>
  constexpr void call(const std::string_view tag, Callee& callee) const
  {
    if (tag == m_tag)
    {
      callee.template call<T>();
    } else {
      m_next.call(tag, callee);
    }
  }

private:
  const NextType m_next;
  const std::string_view m_tag{};
};

//--------------------------------------------------------------------------

struct TCMTailNode
{
  template<class Callee>
  constexpr void call(const std::string_view, Callee&) const
  {
    throw TypeCallMapNotFound{};
  }
};

//--------------------------------------------------------------------------

template<typename T>
constexpr auto operator+(const TCMTailNode& lhs, const TypeCallMapItem<T>& rhs)
{
  return TCMNode<TCMTailNode, T>{lhs, rhs.tag};
}

template<typename T, typename NextNode>
constexpr auto operator+(const NextNode& lhs, const TypeCallMapItem<T>& rhs)
{
  return TCMNode<NextNode, T>{lhs, rhs.tag};
}

//--------------------------------------------------------------------------
} // namespace
//--------------------------------------------------------------------------

/// Create a static type call map.
/// @param items pack of TypeCallMapItem values.
template<class... Items>
inline constexpr auto make_type_call_map(const Items&... items)
{
  return (detail::TCMTailNode{} + ... + items);
}

//==========================================================================
/// @}
//==========================================================================

//==========================================================================
} // namespace

#endif /* include guard */
