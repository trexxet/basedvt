#pragma once

#include <cstdint>
#include <cstdio>
#include <print>
#include <span>

#ifdef __WIN32
#include <windows.h>
#include <io.h>
// bruh
#undef STRICT
#undef DELETE

struct ConfTerm {
	HANDLE hIn;
	DWORD modeSave;

	bool ok = false;
	ConfTerm();
	~ConfTerm();
};
#else
#include <unistd.h>
#include <termios.h>

struct ConfTerm {
	termios save;

	bool saved = false, ok = false;
	ConfTerm();
	~ConfTerm();
};
#endif

ssize_t read_input (std::span<uint8_t> buf);
