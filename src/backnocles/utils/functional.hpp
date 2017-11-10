/// Functional stuff.
///
/// @file

/// TODO deny compilation if types in a call map are not unique

#pragma once
#ifndef FUNCTIONAL_HPP__KG9RFBRT
#define FUNCTIONAL_HPP__KG9RFBRT

#include <functional>
#include <stdexcept>
#include <string_view>
#include <utility>

//==========================================================================
namespace backnocles {
//==========================================================================

/// type trait
template<class F>
struct has_call_operator
{
  // detect regular operator() ... sizeof = 1
  template<typename C>
  static char test(decltype(&C::operator()));

  // worst match ... sizeof = 2
  template<typename C>
  static char (&test(...))[2];

  static constexpr bool value = (sizeof(test<F>(0)) == 1);
};

template<class F>
inline constexpr auto has_call_operator_v = has_call_operator<F>::value;

//==========================================================================

/// Inherit from given callables to provide an aggregated operator().
template<class... Funcs>
struct overload : Funcs...
{
  static_assert((has_call_operator_v<Funcs> && ...),
                "all Funcs must have at least one operator()");

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
  constexpr void call(const std::string_view tag, Callee&& callee) const
  {
    if (tag == m_tag)
    {
      callee.template call<T>();
    } else {
      m_next.call(tag, callee);
    }
  }

  template<class Callee, class UnknownCallback>
  constexpr void call(const std::string_view tag, Callee&& callee,
                      UnknownCallback&& unknown_cb) const
  {
    if (tag == m_tag)
    {
      callee.template call<T>();
    } else {
      m_next.call(tag, callee, unknown_cb);
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
  constexpr void call(const std::string_view, Callee&&) const
  {
    throw TypeCallMapNotFound{};
  }

  template<class Callee, class UnknownCallback>
  constexpr void call(const std::string_view tag, Callee&&,
                      UnknownCallback&& unknown_cb) const
  {
    unknown_cb(tag);
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
