
#ifndef _MANAGE_DB_HH_
# define _MANAGE_DB_HH_

# include <mongo/bson/bson.h>
# include <mongo/client/dbclient.h>

namespace	Spider
{
  class	Header;
  class	Log;

  class	ManageDB
  {
  public:
    ManageDB(std::string ip, std::string name);
    ~ManageDB();

    void	connect();
    bool	addLog(const Header&, const Log&);

  private:
    std::string			_ip;
    std::string			_name;
    bool			_everythingFine;
    mongo::DBClientConnection	_dbClientConnection;
  };
}

#endif
