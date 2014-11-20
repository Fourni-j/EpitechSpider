
#ifndef _APPLICATION_HH_
# define _APPLICATION_HH_

# include <list>
# include "INetworkObserver.hh"
# include "Config.hh"
# include "ManageDB.hh"
# include "Network.hh"

namespace	Spider
{
  class	Config;
  class	TcpCLient;
  class	Client;

  class	Application : public INetworkObserver
  {
  public:
    Application();
    virtual ~Application();

    void		run();
    Config&		getConfig();
    ManageDB&		getManageDB();

    virtual void	onClientConnect(TcpClient*);
    virtual void	onClientDisconnect(TcpClient*);

    void		broadcast(char*, size_t);

  private:
    Config		_config;
    ManageDB		_manageDB;
    Network		_network;
    std::list<Client*>	_clients;
  };
}

#endif
