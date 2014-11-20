
#ifndef _LOG_HH_
# define _LOG_HH_

# include <string>
# include "spider.hh"
# include "Coord.hh"

namespace	Spider
{
  class	Log
  {
  public:
    Log();
    Log(struct s_log);

    std::size_t	getLength();

    /* setter */
    void	setTime(std::int64_t);
    void	setInput(std::uint32_t);
    void	setState(std::uint32_t);
    void	setValue(const std::string&);
    void	setCoord(Coord);
    void	setStruct(struct s_log);
    void	setData(char*);

    /* getter */
    std::int64_t	getTime() const;
    std::uint32_t	getInput() const;
    std::uint32_t	getState() const;
    const std::string&	getValue() const;
    Coord		getCoord() const;
    struct s_log	getStruct() const;
    char*		getData();

  private:
    std::int64_t	_time;
    std::uint32_t	_input;
    std::uint32_t	_state;
    std::string		_value;
    Coord		_coord;
  };
}

#endif
