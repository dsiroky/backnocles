/// Program entry point.
///
/// @file

#include <iostream>
#include <string>

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
    if (line == "add animal hippo")
    {
      std::cout << "hippo added" << '\n';
    }
  }

  return 0;
}
