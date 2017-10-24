/// Program entry point.
///
/// @file

#include <iostream>

//==========================================================================

int main(int, char*[])
{
  std::cin >> std::ws;
  while (true)
  {
    std::string line;
    std::getline(std::cin, line);
    if (line == "quit")
    {
      break;
    }
  }

  return 0;
}
