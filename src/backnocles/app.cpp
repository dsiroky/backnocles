/// User input, output, main loop.
///
/// @file

#include <iostream>
#include <string>

#include "backnocles/app.hpp"

//==========================================================================
namespace backnocles {
//==========================================================================

int App::ui_loop() noexcept
{
  try
  {
    std::cin >> std::ws;
    bool quit{false};
    while (!quit)
    {
      std::string line;
      std::getline(std::cin, line);
      request(line,
              [&](const auto output) { std::cout << output << '\n'; },
              [&](){ quit = true; });
    }
  } catch (const std::exception& exc) {
    std::cerr << "error: " << exc.what() << '\n';
    return -1;
  }

  return 0;
}

//--------------------------------------------------------------------------

void App::request(const std::string_view input,
                  const std::function<void(const std::string_view)> output_cb,
                  const std::function<void()> quit_cb)
{
  if (input == "addanimal hippo")
  {
    output_cb("hippo id:1 added");
  } else {
    output_cb("bye");
    quit_cb();
  }
}

//==========================================================================

AppConstructor::AppConstructor()
  : m_app{}
{
}

//--------------------------------------------------------------------------

int AppConstructor::run() noexcept
{
  return m_app.ui_loop();
}

//==========================================================================
} // namespace
