/// User input, output, main loop.
///
/// @file

#pragma once
#ifndef APP_HPP__4TYTA0ZC
#define APP_HPP__4TYTA0ZC

#include <functional>
#include <string_view>

//==========================================================================
namespace backnocles {
//==========================================================================

class App
{
public:
  /// @return process exit code
  int ui_loop();
  /// Top-level request-response method for user IO.
  void request(const std::string_view input,
                const std::function<void(const std::string_view)> output_cb,
                const std::function<void()> quit_cb);

private:
};

//==========================================================================

/// Main class responsible for building the base application logic.
class AppConstructor
{
public:
  AppConstructor();
  /// @return process exit code.
  int run();

private:
  App m_app;
};

//==========================================================================
} // namespace

#endif /* include guard */
