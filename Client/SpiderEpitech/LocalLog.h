//
//  LocalLog.h
//  AsyncClientBoost
//
//  Created by Charles Fournier on 01/11/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#ifndef __AsyncClientBoost__LocalLog__
#define __AsyncClientBoost__LocalLog__

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "ProtocoleSpider.h"

class LocalLog {
private: 
    std::string		_fileName;
	std::fstream	_fstream;

public:
    LocalLog();
    LocalLog(std::string fileName);
    ~LocalLog();
	void logging(void *data, int size);
	void *getLocalLog(std::fstream &stream);
};

#endif /* defined(__AsyncClientBoost__LocalLog__) */
