//
//  LocalLog.cpp
//  AsyncClientBoost
//
//  Created by Charles Fournier on 01/11/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#include "LocalLog.h"

LocalLog::LocalLog()
{
}

LocalLog::LocalLog(std::string fileName)
{
    _fileName = fileName;
	_fstream.open(_fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
	if (_fstream.is_open())
	{
		std::cout << "Can't create file" << std::endl;
	}
}

void LocalLog::logging(void *data, int size)
{
	_fstream.close();
	_fstream.open("local.spider", std::ios::out | std::ios::binary | std::ios::app);
	_fstream.write((char *)data, size);
	_fstream.close();
}

void *LocalLog::getLocalLog(std::fstream &stream)
{
//	_fstream.close();
//	_fstream.open("local.spider", std::ios::in | std::ios::binary);
//	char log[sizeof(t_log)];
	char *localLog = new char[sizeof(t_log)];
	if (stream.is_open())
	{
		stream.read(localLog, sizeof(t_log));
		if (stream.gcount() == sizeof(t_log))
		{
			return ((void *)localLog);
		}
	}
	return NULL;
}

LocalLog::~LocalLog()
{
	_fstream.close();
}