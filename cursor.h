#pragma once
#include <windows.h>

namespace Cursor
{
	void set(short x, short y) {
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos = { x, y };
		SetConsoleCursorPosition(h, pos);
	}
}