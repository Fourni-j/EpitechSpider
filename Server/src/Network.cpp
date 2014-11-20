
#include "Network.hh"
#include "TcpClient.hh"
#include "INetworkObserver.hh"

namespace	Spider
{
  Network::Network(int port, INetworkObserver& obs)
    : _networkObserver(obs),
      _acceptor(this->_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      _socket(this->_ioService)
  {
    this->accept();
  }

  Network::~Network()
  {
    
  }

  void	Network::run()
  {
    try
      {
	this->_ioService.run();
      }
    catch (const std::exception& e)
      {
	std::cerr << e.what() << std::endl;
      }
  }

  void	Network::accept()
  {
    _acceptor.async_accept(_socket, [this](boost::system::error_code ec)
			   {
			     if (!ec)
			       {
				 TcpClient* newTcpClient = new TcpClient(std::move(_socket), _networkObserver);
				 _networkObserver.onClientConnect(newTcpClient);
				 newTcpClient->start();
			       }
			     accept();
			   });
  }
}

