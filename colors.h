#pragma once
#include <windows.h>

enum Colors {
	white=7, black=8, lwhite = 15, lblack = 0, cursor=6, variant_fg=10, variant_bg=2, checked_fg=4,checked_bg=12
};

void SetColor(Colors fg=white, Colors bg=lblack) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, int(bg)*16+fg);
}