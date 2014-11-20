//
//  Network.cpp
//  AsyncClientBoost
//
//  Created by Charles Fournier on 29/10/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#include "Network.h"
#include <iostream>

void Network::resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it)
{
    if (!ec)
        _m_sock->async_connect(*it, boost::bind(&Network::connect_handler, this, boost::asio::placeholders::error));
    else
    {
        std::cout<< ec.message() << std::endl;
        _isConnected = false;
		notify("DISCONNECTED");
	}
}

void Network::connect_handler(const boost::system::error_code &ec)
{
    if (!ec)
    {
        _isConnected = true;
		notify("CONNECTED");
		std::cout << "Connected to " << _serverIP << std::endl;
        _m_sock->async_read_some(boost::asio::buffer(_buffer), boost::bind(&Network::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        std::cout << ec.message() << std::endl;
        _isConnected = false;
		notify("DISCONNECTED");
	}

}

void Network::send_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if (ec)
    {
        _isConnected = false;
        std::cout << ec.message() << std::endl;
		notify("DISCONNECTED");
	}
}

void Network::setServerIP(std::string ip)
{
	_serverIP = ip;
}

void Network::read_handler(const boost::system::error_code &ec,
                  std::size_t bytes_transferred)
{
    if (!ec)
    {
        this->notify(_buffer.data(), bytes_transferred);
        _m_sock->async_read_some(boost::asio::buffer(_buffer), boost::bind(&Network::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        std::cout << ec.message() << std::endl;
        _isConnected = false;
		notify("DISCONNECTED");
	}
}

Network::Network()
{
    _isConnected = false;
}

Network::Network(IObserver *observer)
{
    this->addObserver(observer);
	_isConnected = false;
}

Network::~Network()
{
    
}

void Network::run(std::string ip = "YOUR_IP_ADDRESS")
{
    try
    {
		_serverIP = ip;
		_shared_io_service.reset(new boost::asio::io_service);
		_m_sock.reset(new boost::asio::ip::tcp::socket(*_shared_io_service));
		boost::asio::ip::tcp::resolver resolver(*_shared_io_service);
		boost::asio::ip::tcp::resolver::query query(ip, "4243");
		resolver.async_resolve(query, boost::bind(&Network::resolve_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
        _shared_io_service->run();
    }
    catch (std::exception const &e)
    {
        std::cout << e.what() << std::endl;
		notify("DISCONNECTED");
	}
}

void Network::stop()
{
	_m_sock->close();
	_shared_io_service->stop();
	_m_sock.reset();
	_shared_io_service.reset();
	notify("DISCONNECTED");
	_isConnected = false;
}

void Network::sendData(const void *data, const int &size)
{
	_shared_io_service->post(NetworkData(this, data, size));
}

void Network::sendData(std::string const &data)
{
    if (_isConnected == true) {
    _m_sock->async_send(boost::asio::buffer(data.c_str(), data.size()), boost::bind(&Network::send_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
}

void Network::safeSend(const void *data, const int &size)
{
	if (_isConnected == true) {
		_m_sock->async_send(boost::asio::buffer(data, size), boost::bind(&Network::send_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

bool Network::isConnected() const
{
    return _isConnected;
}

NetworkData::NetworkData(Network *net, const void *data, const int &size)
{
	_net = net;
	_size = size;
	_data = new (char[size]);
	std::memcpy(_data, data, size);
}

NetworkData::NetworkData(const NetworkData &data) 
{
	_net = data.getNetwork();
	_size = data.getSize();
	_data = new (char[_size]);
	std::memcpy(_data, data.getData(), _size);
}

NetworkData::~NetworkData()
{
	delete (_data);
}

void NetworkData::operator()()
{
	_net->safeSend(_data, _size);
}

char *NetworkData::getData() const
{
	return _data;
}

int NetworkData::getSize() const
{
	return _size;
}

Network *NetworkData::getNetwork() const
{
	return _net;
}

