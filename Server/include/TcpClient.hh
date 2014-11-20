
#ifndef _TCP_CLIENT_HH_
# define _TCP_CLIENT_HH_

# include <deque>
# include <utility>
# include <boost/asio.hpp>

namespace	Spider
{
  class	Client;
  class	INetworkObserver;

  class	TcpClient
  {
  public:
    TcpClient(boost::asio::ip::tcp::socket, INetworkObserver&);
    ~TcpClient();

    void	start();
    void	setObserver(Client*);
    Client*	getObserver() const;
    void	close();
    void	setTimeout(int);

    void	deliver(char*, size_t);
    void	read(char*, size_t);

  private:
    void	write();
    void	timeoutExpired(const boost::system::error_code&);

  private:
    boost::asio::ip::tcp::socket		_socket;
    boost::asio::deadline_timer			_timer;
    Client*					_clientObserver;
    INetworkObserver&				_networkObserver;
    std::deque<std::pair<char*, size_t> >	_writeMsgs;
    char*					_actualData;
    size_t					_actualSize;
    int						_timeout;
  };
}

#endif
