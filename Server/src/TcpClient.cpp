
#include <iostream>
#include <cstring>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include "TcpClient.hh"
#include "Client.hh"
#include "INetworkObserver.hh"

namespace	Spider
{
  TcpClient::TcpClient(boost::asio::ip::tcp::socket socket, INetworkObserver& obs)
    : _socket(std::move(socket)),
      _timer(_socket.get_io_service()),
      _networkObserver(obs)
  {
    _clientObserver = NULL;
    _actualData = new char[128];
    memset(_actualData, '\0', 128);
    _actualSize = 0;
    _timeout = 1;
    _timer.expires_from_now(boost::posix_time::seconds(_timeout));
    _timer.async_wait(boost::bind(&TcpClient::timeoutExpired, this, boost::asio::placeholders::error));
  }

  TcpClient::~TcpClient()
  {
    _timer.cancel();
  }

  void	TcpClient::start()
  {
    _clientObserver->onClientStart();
    this->read(_actualData, 20);
  }
  
  void	TcpClient::setObserver(Client* clientObs)
  {
    _clientObserver = clientObs;
  }

  Client*	TcpClient::getObserver() const
  {
    return (_clientObserver);
  }

  void	TcpClient::close()
  {
    _socket.close();
  }

  void	TcpClient::setTimeout(int timeout)
  {
    _timeout = timeout;
  }

  void	TcpClient::deliver(char* data, size_t size)
  {
    bool	writeInProgress = !_writeMsgs.empty();

    _writeMsgs.push_back(std::make_pair(data, size));
    if (!writeInProgress)
      this->write();
  }

  void	TcpClient::read(char *data, std::size_t size)
  {
    _actualData = data;
    _actualSize = size;
    boost::asio::async_read(_socket,
			    boost::asio::buffer(_actualData, _actualSize),
			    [this](boost::system::error_code ec, std::size_t)
			    {
			      if (!ec)
				{

				  _timer.cancel();
				  _timer.expires_from_now(boost::posix_time::seconds(_timeout));
				  _timer.async_wait(boost::bind(&TcpClient::timeoutExpired, this, boost::asio::placeholders::error));

				  _clientObserver->onMessageRead(_actualData, _actualSize);
				}
			      else
				_networkObserver.onClientDisconnect(this);
			    });
  }

  void	TcpClient::write()
  {
    boost::asio::async_write(_socket,
			     boost::asio::buffer(_writeMsgs.front().first,
						 _writeMsgs.front().second),
			     [this](boost::system::error_code ec, std::size_t)
			     {
			       if (!ec)
				 {
				   // delete [] (_writeMsgs.front().first);
				   _writeMsgs.pop_front();
				   if (!_writeMsgs.empty())
				     this->write();
				 }
			     });
  }

  void	TcpClient::timeoutExpired(const boost::system::error_code& ec)
  {
    if (!ec)
      {
	_clientObserver->onTimeoutExpired();
	_timer.expires_from_now(boost::posix_time::seconds(_timeout));
	_timer.async_wait(boost::bind(&TcpClient::timeoutExpired, this, boost::asio::placeholders::error));
      }
  }
}
