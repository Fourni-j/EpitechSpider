
#ifndef _CLIENT_HH_
# define _CLIENT_HH_

# include <map>
# include "ITcpClientObserver.hh"
# include "Header.hh"
# include "Log.hh"

namespace	Spider
{
  class	TcpClient;
  class	Config;
  class	Application;

  class	Client : public ITcpClientObserver
  {
  public:
    Client(TcpClient*, Application&);
    virtual ~Client();

    void		send(char*, size_t);

    virtual void	onClientStart();
    virtual void	onMessageRead(char*, size_t);
    virtual void	onTimeoutExpired();

  private:
    void	manageLogType();
    void	manageIdType();
    void	manageRespKoType();
    void	manageRespOkType();
    void	managePauseType();
    void	manageRestartType();
    void	manageMoveType();
    void	manageDeleteType();
    void	managePingType();
    void	managePongType();
    void	manageOtherType();

    bool	isValidKeyAndToken();
    int32_t	generateUniqueId();
    uint32_t	generateRandomToken();

  private:
    TcpClient*					_tcpClient;
    Application&				_app;
    Header					_actualHeader;
    Log						_actualLog;
    int32_t					_idClient;
    bool					_getPong;
    std::map<uint32_t, void(Client::*)()>	_functions;
  };
}

#endif
