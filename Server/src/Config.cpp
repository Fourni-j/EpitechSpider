
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "Config.hh"

namespace	Spider
{
  Config::Config() : _readerIni("conf/spider.ini")
  {
    _root = Json::nullValue;
    this->parsePrimordial();
    this->parseUsers();
  }

  /* primordial rules */
  int	Config::getServerPort()
  {
    return (_readerIni.GetInteger("server", "port", -1));
  }

  std::string	Config::getProtocolName()
  {
    return (_readerIni.Get("protocol", "name", "UNKNOWN"));
  }

  uint32_t	Config::getProtocolVersion()
  {
    return (_readerIni.GetInteger("protocol", "version", -1));
  }

  uint32_t	Config::getProtocolSubversion()
  {
    std::string	sub = _readerIni.Get("protocol", "version", "UNKNOWN");

    return (std::atoi(sub.substr(sub.find('.', 0) + 1, sub.size() - sub.find('.', 0) - 1).c_str()));
  }

  uint32_t	Config::getProtocolMagicNumber()
  {
    return (_readerIni.GetInteger("protocol", "magicNb", -1));
  }

  /* users rules */
  bool	Config::isIdDefined(int32_t key)
  {
    if (_keysAndTokens.find(key) != _keysAndTokens.end())
      return (true);
    return (false);
  }

  uint32_t	Config::getTokenById(int32_t id)
  {
    return (_keysAndTokens[id]);
  }

  bool	Config::addUser(int32_t id, uint32_t token)
  {
    std::ofstream	file("conf/users.json");
    Json::Value		newUser(Json::objectValue);
    Json::StyledWriter	writer;
    Json::Value::UInt	i = _root["users"].size();

    if (!file.fail())
      {
    	if ((newUser["id"] = id) == Json::nullValue)
    	  return (false);
    	if ((newUser["token"] = token) == Json::nullValue)
    	  return (false);
    	if ((_root["users"][i] = newUser) == Json::nullValue)
    	  return (false);
    	file << writer.write(_root);
    	file.close();
    	return (true);
      }
    else
      {
    	std::cerr << "can't open the file" << std::endl;
    	return (false);
      }
  }

  /* database rules */

  std::string	Config::getDBIp()
  {
    return (_readerIni.Get("database", "server", "localhost"));
  }

  std::string	Config::getDBName()
  {
    return (_readerIni.Get("database", "name", "spider"));
  }

  /* parsing */
  void	Config::parsePrimordial()
  {
    if (_readerIni.ParseError() > 0)
      {
	std::cerr << "\033[31mCan't load primordial configuration for the server.\033[0m" << std::endl;
	exit(-1);
      }
  }

  void	Config::parseUsers()
  {
    std::ifstream	file("conf/users.json");

    if(!file.fail())
      {
	bool parsingSuccessful = _readerJson.parse(file, _root, false);
    
	if (!parsingSuccessful)
	  {
	    std::cerr  << "\033[31mFailed to parse users file\n\033[36m"
		       << _readerJson.getFormatedErrorMessages()
		       << "\033[0m" << std::endl;
	    _root = Json::Value(Json::objectValue);
	    Json::Value	users(Json::arrayValue);
	    Json::Value::UInt	i = 0;
	    _root[i] = users;
	  }
	file.close();
      }
    else
      {
	std::cerr << "\033[31mCan't find users file\033[0m" << std::endl;
      }

    /* je remplie ma map avec les utilisateurs */
    for (size_t i = 0; i < _root["users"].size(); i++)
      {
	_keysAndTokens[reinterpret_cast<int32_t>(_root["users"][i]["id"].asInt())] = reinterpret_cast<uint32_t>(_root["users"][i]["token"].asUInt());
      }
  }
}
