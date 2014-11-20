
#ifndef _NETWORK_OBSERVER_HH_
# define _NETWORK_OBSERVER_HH_

namespace	Spider
{
  class	TcpClient;

  class	INetworkObserver
  {
  public:
    virtual	~INetworkObserver() {};

    virtual void	onClientConnect(TcpClient*) = 0;
    virtual void	onClientDisconnect(TcpClient*) = 0;
  };
}

#endif
