
#include <iostream>
#include <exception>
#include "Application.hh"

int	main()
{
  try
    {
      Spider::Application	server;

      server.run();
    }
  catch (const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  return (0);
}
