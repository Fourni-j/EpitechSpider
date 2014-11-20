//
//  IObservable.h
//  AsyncClientBoost
//
//  Created by Charles Fournier on 01/11/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#ifndef __AsyncClientBoost__IObservable__
#define __AsyncClientBoost__IObservable__

#include <stdio.h>
#include <string>
#include <list>
#include "IObserver.h"

class IObservable {
private:
    std::list<IObserver*> _list_observers;
    
public:
    void notify(std::string data) const
    {
        // Notifier tous les observers
        for (std::list<IObserver*>::const_iterator it = _list_observers.begin();
             it != _list_observers.end(); ++it)
            (*it)->update(data);
    }

	void notify(void *data, int size) const
	{
		for (std::list<IObserver*>::const_iterator it = _list_observers.begin();
			it != _list_observers.end(); ++it)
			(*it)->update(data, size);
	}
    
	void notify(Data *data) const
	{
		for (std::list<IObserver*>::const_iterator it = _list_observers.begin();
			it != _list_observers.end(); ++it) {
			(*it)->update(data);
			std::cout << "notify" << std::endl;
		}
	}

    void addObserver(IObserver* observer)
    {
        // Ajouter un observer a la liste
        _list_observers.push_back(observer);
    }
    
    void removeObserver(IObserver* observer)
    {
        // Enlever un observer a la liste
        _list_observers.remove(observer);
    }
};


#endif /* defined(__AsyncClientBoost__IObservable__) */
