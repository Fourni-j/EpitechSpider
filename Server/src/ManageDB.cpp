
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <exception>
#include "ManageDB.hh"
#include "Header.hh"
#include "Log.hh"

namespace	Spider
{
  ManageDB::ManageDB(std::string ip, std::string name)
    : _ip(ip),
      _name(name)
  {
    if (mongo::client::initialize() != mongo::Status::OK())
      _everythingFine = false;
    else
      _everythingFine = true;
  }

  ManageDB::~ManageDB()
  {
  }

  void	ManageDB::connect()
  {
    if (_everythingFine == false)
      return ;

    try
      {
	_dbClientConnection.connect(_ip);
      }
    catch (const std::exception& e)
      {
	std::cerr << e.what() << std::endl;
	_everythingFine = false;
      }
  }

  bool	ManageDB::addLog(const Header& header, const Log& log)
  {
    if (_everythingFine == false)
      return (false);

    mongo::BSONObjBuilder	builder;
    mongo::BSONObj		obj;
    std::string			str1;
    std::string			str2;
    std::stringstream		str;

    str1 = boost::lexical_cast<std::string>(header.getIdClient());
    str2 = boost::lexical_cast<std::string>(header.getTokenId());
    str << _name << ".log" << str1 << "_" << str2;

    builder.genOID(); /* assigne un id unique */
    builder.append("time", static_cast<long long int>(log.getTime()));
    builder.append("input", log.getInput());
    builder.append("state", log.getState());
    builder.append("value", log.getValue());
    builder.append("coord_x", log.getCoord().getX());
    builder.append("coord_y", log.getCoord().getY());

    obj = builder.obj();
    _dbClientConnection.insert(str.str().c_str(), obj);
    if (_dbClientConnection.getLastError() != "")
      {
	std::cerr << _dbClientConnection.getLastError() << std::endl;
	return false;
      }

    return true;
  }
}
