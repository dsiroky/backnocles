/// Program entry point.
///
/// @file

#include <memory>

#include "backnocles/app.hpp"

//==========================================================================

int main(int, char*[])
{
  // place the app instance on the heap instead on the stack
  auto app = std::make_unique<backnocles::AppConstructor>();
  return app->run();
}
