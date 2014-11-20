
#ifndef _CONFIG_HH_
# define _CONFIG_HH_

# include <map>
# include <vector>
# include <string>
# include <jsoncpp/json/json.h>
# include "INIReader.h"

namespace	Spider
{
  class	Config
  {
  public:
    Config();

    /* primordial rules */
    int		getServerPort();
    std::string	getProtocolName();
    uint32_t	getProtocolVersion();
    uint32_t	getProtocolSubversion();
    uint32_t	getProtocolMagicNumber();

    /* users rules */
    bool	isIdDefined(int32_t);
    uint32_t	getTokenById(int32_t);
    bool	addUser(int32_t, uint32_t);

    /* database rules */
    std::string	getDBIp();
    std::string	getDBName();
    
  private:
    void	parsePrimordial();
    void	parseUsers();

    INIReader			_readerIni;    
    Json::Reader		_readerJson;
    Json::Value			_root;
    std::map<int32_t, uint32_t>	_keysAndTokens;
  };
}

#endif
