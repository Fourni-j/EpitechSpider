//
//  Data.cpp
//  AsyncClientBoost
//
//  Created by Charles Fournier on 01/11/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#include "Data.h"

int g_id = 0;

Data::Data(KBDLLHOOKSTRUCT hooked, WPARAM wParam)
{
	DWORD key = 1;

	key += hooked.scanCode << 16;
	key += hooked.flags << 24;
	_header.type = LOG;
	_header.size = sizeof(t_log);
	_header.id_msg = g_id;
	_header.id_client = -1;
	_header.token = 0;
	g_id++;
	
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		_log.state = PUSH;
	else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		_log.state = RELEASE;

	_log.time = getTimestamp();
	GetKeyNameTextA(key, _log.value, 64);
	_log.input = KEYSTROKE;
	_log.coord.x = 0;
	_log.coord.y = 0;
	_haveLog = true;

	_timer = time(NULL);
	_encoded = false;
	_logEncoded = false;
}

Data::Data(char *focus)
{
	_header.type = LOG;
	_header.size = sizeof(t_log);
	_header.id_msg = g_id;
	_header.id_client = -1;
	_header.token = 0;
	g_id++;

	_log.time = getTimestamp();
	_log.input = FOCUS;
	_log.coord.x = 0;
	_log.coord.y = 0;
	memcpy(_log.value, focus, sizeof(_log.value));
	_haveLog = true;

	_timer = time(NULL);
	_encoded = false;
	_logEncoded = false;
}


Data::Data(MSLLHOOKSTRUCT hooked, WPARAM wParam)
{
	_header.type = LOG;
	_header.size = sizeof(t_log);
	_header.id_msg = g_id;
	_header.id_client = -1;
	_header.token = 0;
	g_id++;

	if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
		_log.state = PUSH;
	else if (wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP)
		_log.state = RELEASE;
	else
		_log.state = NONE;

	_log.time = getTimestamp();
	_log.input = MOUSE;
	
	_log.coord.x = hooked.pt.x;
	_log.coord.y = hooked.pt.y;
	if (wParam == WM_LBUTTONDOWN ||
		wParam == WM_LBUTTONUP)
		strcpy_s(_log.value, "LEFT");
	else if (wParam == WM_RBUTTONDOWN ||
		wParam == WM_RBUTTONUP)
		strcpy_s(_log.value, "RIGHT");
	else
		strcpy_s(_log.value, "MOVE");
	_haveLog = true;
	_timer = time(NULL);
	_encoded = false;
	_logEncoded = false;
}

Data::Data(void *logPtr)
{
	_header.type = LOG;
	_header.size = sizeof(t_log);
	_header.id_msg = g_id;
	_header.id_client = -1;
	_header.token = 0;
	g_id++;
	memcpy(&_log, logPtr, sizeof(t_log));
	_logEncoded = true;
	_encoded = false;
	_timer = time(NULL);
	_haveLog = true;
}

Data::Data()
{
	_header.type = ID;
	_header.size = 0;
	_header.id_msg = g_id;
	_header.id_client = -1;
	_header.token = 0;
	_haveLog = false;
	g_id++;
	_timer = time(NULL);
	_encoded = false;
}

Data::Data(const e_type type)
{
	_header.type = type;
	_header.size = 0;
	_header.id_msg = g_id;
	_header.id_client = -1;
	_header.token = 0;
	_haveLog = false;
	g_id++;
	_timer = time(NULL);
	_encoded = false;
}

Data::~Data()
{
    
}

bool Data::isEncoded() {
	return _encoded;
}

t_size Data::getHeaderSize()
{
	return sizeof(_header);
}

t_size Data::getLogSize()
{
	return sizeof(_log);
}

t_header *Data::getHeader()
{
	return &_header;
}

t_log *Data::getLog()
{
	return &_log;
}

bool Data::haveLog()
{
	return _haveLog;
}

time_t Data::getTimer()
{
	return _timer;
}

int64_t Data::getTimestamp() {
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::high_resolution_clock::now().time_since_epoch()
		);
	int64_t eventTimestamp = ms.count();
	return eventTimestamp;
}

void Data::encode()
{
	showData();
	if (_encoded == false) {
		_header.id_client = static_cast<int32_t>(htonl(_header.id_client));
		_header.id_msg = htonl(_header.id_msg);
		_header.size = htonl(_header.size);
		_header.token = htonl(_header.token);
		_header.type = htonl(_header.type);
		if (_haveLog == true && _logEncoded == false) {
			_log.coord.x = htonl(_log.coord.x);
			_log.coord.y = htonl(_log.coord.y);
			_log.input = htonl(_log.input);
			_log.state = htonl(_log.state);
			_log.time = _log.time;
			_logEncoded = true;
		}
		_encoded = true;
	}
}

void Data::showData()
{
	std::cout << "-- Header Struct  --" << std::endl;
	std::cout << "id_Client : " << _header.id_client << std::endl;
	std::cout << "id_msg : " << _header.id_msg << std::endl;
	std::cout << "size : " << _header.size << std::endl;
	std::cout << "token : " << _header.token << std::endl;
	std::cout << "type : " << _header.type << std::endl;
	std::cout << "-- End --" << std::endl;
	if (_haveLog == true) {
		std::cout << "-- Log Struct --" << std::endl;
		std::cout << "time :" << _log.time << std::endl;
		std::cout << "input :" << _log.input << std::endl;
		std::cout << "state :" << _log.state << std::endl;
		std::cout << "value :" << _log.value << std::endl;
		std::cout << "coord :" << _log.coord.x << "/" << _log.coord.y << std::endl;
		std::cout << "-- End --" << std::endl;
	}
}

void Data::decode(t_magic_header *header)
{
	header->magic = ntohl(header->magic);
	header->major = ntohl(header->major);
	header->minor = ntohl(header->minor);
}

void Data::decode(t_header *header)
{
	header->id_client = static_cast<int32_t>(ntohl(header->id_client));
	header->id_msg = ntohl(header->id_msg);
	header->size = ntohl(header->size);
	header->token = ntohl(header->token);
	header->type = ntohl(header->type);
}

void Data::display(t_magic_header *header)
{
	std::cout << "-- Magic Header Struct --" << std::endl;
	std::cout << "name : " << header->name << std::endl;
	std::cout << "magic : " << header->magic << std::endl;
	std::cout << "major : " << header->major << std::endl;
	std::cout << "minor : " << header->minor << std::endl;
	std::cout << "-- End --" << std::endl;
}

void Data::display(t_header *header)
{
	std::cout << "-- Header Struct --" << std::endl;
	std::cout << "id_client : " << header->id_client << std::endl;
	std::cout << "token : " << header->token << std::endl;
	std::cout << "type : " << header->type << std::endl;
	std::cout << "id_msg : " << header->id_msg << std::endl;
	std::cout << "size : " << header->size << std::endl;
	std::cout << "-- End --" << std::endl;
}