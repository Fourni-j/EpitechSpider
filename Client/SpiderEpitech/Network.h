//
//  Network.h
//  AsyncClientBoost
//
//  Created by Charles Fournier on 29/10/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#ifndef __AsyncClientBoost__Network__
#define __AsyncClientBoost__Network__

#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "IObservable.h"

class Network : public IObservable
{
private:
    bool                            _isConnected;
    boost::array<char, 4096>        _buffer;
    boost::shared_ptr<boost::asio::io_service>      _shared_io_service;
    boost::shared_ptr<boost::asio::ip::tcp::socket> _m_sock;
	std::string		_serverIP;

public:
    Network();
    Network(IObserver *observer);
    ~Network();
    void run(std::string ip);
	void stop();
    void sendData(const void *data, int const &size);
    void sendData(std::string const &data);
    bool isConnected() const;
	void safeSend(const void *data, int const &size);
	void setServerIP(std::string ip);

private:
    void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);
    void connect_handler(const boost::system::error_code &ec);
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
    void send_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
};


class NetworkData {
public:
	NetworkData(Network *net, const void *data, const int &size);
	NetworkData(const NetworkData &data);
	~NetworkData();

	void operator()();
	char *getData() const;
	Network *getNetwork() const;
	int getSize() const;

private:
	char	*_data;
	int		_size;
	Network *_net;
};
#endif /* defined(__AsyncClientBoost__Network__) */
