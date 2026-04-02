#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

class MakcuManager
{
public:
	HANDLE hSerial;

	int init(int com_port);

	void km_move(int X, int Y);

	void km_move_auto(int X, int Y, int runtime);

	void km_click();

private:
	bool open_port(HANDLE& hSerial, const char* portName, DWORD baudRate);
	void send_command(HANDLE hSerial, const std::string& command);
};

inline MakcuManager makcuMgr;
