#include "Makcu.h"

bool MakcuManager::open_port(HANDLE& hSerial, const char* portName, DWORD baudRate) {
	hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hSerial == INVALID_HANDLE_VALUE) return false;

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hSerial, &dcbSerialParams)) {
		CloseHandle(hSerial);
		return false;
	}

	dcbSerialParams.BaudRate = baudRate;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(hSerial, &dcbSerialParams)) {
		CloseHandle(hSerial);
		return false;
	}

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 5;
	timeouts.ReadTotalTimeoutConstant = 5;
	timeouts.ReadTotalTimeoutMultiplier = 5;
	timeouts.WriteTotalTimeoutConstant = 5;
	timeouts.WriteTotalTimeoutMultiplier = 5;

	if (!SetCommTimeouts(hSerial, &timeouts)) {
		std::cerr << "Error setting timeouts!" << std::endl;
		CloseHandle(hSerial);
		return false;
	}

	return true;
}

void MakcuManager::send_command(HANDLE hSerial, const std::string& command) {
	DWORD bytesWritten;
	if (!WriteFile(hSerial, command.c_str(), command.length(), &bytesWritten, NULL)) {
		std::cerr << "Failed to write to serial port!" << std::endl;
	}
}

int MakcuManager::init(int com_port)
{
	if (com_port <= 0 || com_port > 256) {
		return -1;
	}
	
	std::string portStr = "\\\\.\\COM" + std::to_string(com_port);
	
	if (!open_port(hSerial, portStr.c_str(), CBR_115200)) { // CBR_115200 is the baud rate
		return -2;
	}
	return 0;
}

void MakcuManager::km_move(int X, int Y)
{
	std::string command = "km.move(" + std::to_string(X) + "," + std::to_string(Y) + ")\r\n";
	send_command(hSerial, command.c_str());
}

void MakcuManager::km_move_auto(int X, int Y, int runtime)
{
	std::string command = "km.move(" + std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(runtime) + ")\r\n";
	send_command(hSerial, command.c_str());
}

void MakcuManager::km_click()
{
	std::string command = "km.left(" + std::to_string(1) + ")\r\n"; // left mouse button down
	Sleep(10); // to stop it from crashing idk
	std::string command1 = "km.left(" + std::to_string(0) + ")\r\n"; // left mouse button up
	send_command(hSerial, command.c_str());
	send_command(hSerial, command1.c_str());
}
