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
  struct size_one_t { char x; };
  struct size_two_t { char x[2]; };

  // detect regular operator() ... sizeof = 1
  template<typename C>
  static size_one_t test(decltype(&C::operator()));

  // worst match ... sizeof = 2
  template<typename C>
  static size_two_t test(...);

  static constexpr bool value = (sizeof(test<F>(0)) == 1);
};

//==========================================================================

// MSVC2017 does not support [Pack expansions in
// using-declarations](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0195r2.html)
// and folding expressions. This has to be implemented the old fashioned
// recursive way.

/// Inherit from given callables to provide an aggregated operator().
template<class F, class... Funcs>
struct overload : F, overload<Funcs...>
{
  static_assert(has_call_operator<F>::value,
                "all Funcs must have at least one operator()");

  constexpr overload(F&& f, Funcs&&... fs)
    : F{std::forward<F>(f)}, overload<Funcs...>{std::forward<Funcs>(fs)...}
  {}

  using F::operator();
  using overload<Funcs...>::operator();
};

template<class F>
struct overload<F> : F
{
  static_assert(has_call_operator<F>::value,
                "all Funcs must have at least one operator()");

  constexpr overload(F&& f)
    : F{std::forward<F>(f)}
  {}

  using F::operator();
};

//--------------------------------------------------------------------------

/// Overload maker.
/// Example:
/// @code
/// auto f = backnocles::make_overload(
///               [&](char v) { std::cout << "char " << v << '\n';},
///               [&](int v) { std::cout << "int " << v << '\n';},
///               [&](float*) { std::cout << "float ptr" << '\n'; }
///             );
/// f(4); // prints "int 4"
/// f('a'); // prints "char a"
/// @endcode
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
template<class I, class... Items>
inline constexpr auto make_type_call_map(const I& item, const Items&... items)
{
  return make_type_call_map(items...) + item;
}

template<class I>
inline constexpr auto make_type_call_map(const I& item)
{
  return detail::TCMTailNode{} + item;
}

inline constexpr auto make_type_call_map()
{
  return detail::TCMTailNode{};
}

//==========================================================================
/// @}
//==========================================================================

//==========================================================================
} // namespace

#endif /* include guard */
