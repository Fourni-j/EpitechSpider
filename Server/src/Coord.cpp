
#include <arpa/inet.h>
#include "Coord.hh"

namespace	Spider
{
  Coord::Coord()
  {
    _x = 0; 
    _y = 0;
  }

  Coord::Coord(struct s_coord coord)
  {
    _x = ntohl(coord.x);
    _y = ntohl(coord.y);
  }

  /* setter */
  void	Coord::setX(std::int32_t x) { _x = x; }
  void	Coord::setY(std::int32_t y) { _y = y; }

  void	Coord::setStruct(struct s_coord coord)
  {
    _x = ntohl(coord.x);
    _y = ntohl(coord.y);
  }

  /* getter */
  std::int32_t	Coord::getX() const { return (_x); }
  std::int32_t	Coord::getY() const { return (_y); }

  struct s_coord	Coord::getStruct() const
  {
    struct s_coord	resp;

    resp.x = htonl(_x);
    resp.y = htonl(_y);
    return (resp);
  }
}
