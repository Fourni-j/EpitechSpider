
#ifndef _NETWORK_HH_
# define _NETWORK_HH_

# include <boost/asio.hpp>
# include "INetworkObserver.hh"

namespace	Spider
{
  class	Network
  {
  public:
    Network(int port, INetworkObserver&);
    ~Network();

    void	run();
    void	accept();

  private:
    INetworkObserver&			_networkObserver;
    boost::asio::io_service		_ioService;
    boost::asio::ip::tcp::acceptor	_acceptor;
    boost::asio::ip::tcp::socket	_socket;
  };
}

#endif
