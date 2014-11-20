
#ifndef _COORD_HH_
# define _COORD_HH_

# include "spider.hh"

namespace	Spider
{
  class	Coord
  {
  public:
    Coord();
    Coord(struct s_coord);

    /* setter */
    void	setX(std::int32_t);
    void	setY(std::int32_t);
    void	setStruct(struct s_coord);

    /* getter */
    std::int32_t	getX() const;
    std::int32_t	getY() const;
    struct s_coord	getStruct() const;

  private:
    std::int32_t	_x;
    std::int32_t	_y;
  };
}

#endif
