
#include <iostream>
#include "Application.hh"
#include "Config.hh"
#include "Client.hh"
#include "TcpClient.hh"

namespace	Spider
{
  Application::Application() :
    _config(),
    _manageDB(_config.getDBIp(), _config.getDBName()),
    _network(_config.getServerPort(), *this)
  {
    _manageDB.connect();
  }

  Application::~Application()
  {

  }

  void	Application::run()
  {
    _network.run();
  }

  Config&	Application::getConfig()
  {
    return (_config);
  }

  ManageDB&	Application::getManageDB()
  {
    return (_manageDB);
  }

  void	Application::onClientConnect(TcpClient* tcpClient)
  {
    Client*	client = new Client(tcpClient, *this);

    tcpClient->setObserver(client);
    _clients.push_back(client);
    std::cout << "WOUHOU !! THERE IS A NEW CLIENT !\n"
	      << "There is now " << _clients.size() << " client(s) on the server." << std::endl;
  }

  void	Application::onClientDisconnect(TcpClient* tcpClient)
  {
    std::cout << "NOOOOOO !! ONE OF THE CLIENTS DIE !" << std::endl;
    delete (tcpClient->getObserver());
    _clients.remove(tcpClient->getObserver());
    delete (tcpClient);
  }

  void	Application::broadcast(char* data, size_t size)
  {
    std::list<Client*>::iterator	it;

    for (it = _clients.begin(); it != _clients.end(); it++)
      (*it)->send(data, size);
  }
}
