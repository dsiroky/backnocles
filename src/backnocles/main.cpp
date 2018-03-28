/// Program entry point.
///
/// @file

#include "backnocles/app.hpp"

//==========================================================================

int main(int, char*[])
{
  // avoid placing the app instance on the stack
  static backnocles::AppConstructor app{};
  return app.run();
}
