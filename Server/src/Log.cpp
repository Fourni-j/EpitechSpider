
#include <arpa/inet.h>
#include <cstring>
#include "Log.hh"

namespace	Spider
{
  Log::Log()
  {
    _time = 0;
    _input = 0;
    _state = 0;
    _value = "";
  }

  Log::Log(struct s_log structure)
  {
    // int	num = 42;

    // if (*reinterpret_cast<char*>(&num) == num)
    //   _time = htole64(structure.time);
    // else
    //   _time = htobe64(structure.time);
    _time = structure.time;
    _input = ntohl(structure.input);
    _state = ntohl(structure.state);
    _value = structure.value;
    _coord.setStruct(structure.coord);
  }

  std::size_t	Log::getLength()
  {
    return (sizeof(struct s_log));
  }

  /* setter */
  void	Log::setTime(std::int64_t time) { _time = time; }
  void	Log::setInput(std::uint32_t input) { _input = input; }
  void	Log::setState(std::uint32_t state) { _state = state; }
  void	Log::setValue(const std::string& value) { _value = value; }
  void	Log::setCoord(Coord coord) { _coord = coord; }

  void	Log::setStruct(struct s_log structure)
  {
    // int	num = 42;

    // if (*reinterpret_cast<char*>(&num) == num)
    //   _time = htole64(structure.time);
    // else
    //   _time = htobe64(structure.time);
    _time = structure.time;
    _input = ntohl(structure.input);
    _state = ntohl(structure.state);
    _value = structure.value;
    _coord.setStruct(structure.coord);
  }

  void	Log::setData(char* data)
  {
    struct s_log	log;

    memcpy(&log, data, this->getLength());
    this->setStruct(log);
  }

  /* getter */
  std::int64_t		Log::getTime() const { return (_time); }
  std::uint32_t		Log::getInput() const { return (_input); }
  std::uint32_t		Log::getState() const { return (_state); }
  const std::string&	Log::getValue() const { return (_value); }
  Coord			Log::getCoord() const { return (_coord); }

  struct s_log	Log::getStruct() const
  {
    struct s_log	resp;

    // resp.time = be64toh(_time);
    resp.time = _time;
    resp.input = htonl(_input);
    resp.state = htonl(_state);
    memset(resp.value, '\0', sizeof(resp.value));
    for (size_t i = 0; i < _value.size(); i++)
      resp.value[i] = _value[i];
    resp.coord = _coord.getStruct();
    return (resp);
  }

  char*	Log::getData()
  {
    char*		data = new char[this->getLength()];
    struct s_log	*tmp = new struct s_log;

    *tmp = this->getStruct();
    memcpy(data, tmp, this->getLength());
    delete (tmp);
    return (data);
  }
}
