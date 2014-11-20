//
//  IObserver.h
//  AsyncClientBoost
//
//  Created by Charles Fournier on 01/11/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#ifndef __AsyncClientBoost__IObserver__
#define __AsyncClientBoost__IObserver__

#include <stdio.h>
#include <string>
#include "Data.h"

class IObserver {

public:
    virtual void update(std::string data) = 0;
	virtual void update(void *data, int size) = 0;
	virtual void update(Data *data) = 0;
};

#endif /* defined(__AsyncClientBoost__IObserver__) */
