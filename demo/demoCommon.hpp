#pragma once

#include <cstdint>
#include <cstdio>
#include <print>
#include <span>

// TODO: linux

#ifdef __WIN32
#include <windows.h>
#include <io.h>
// bruh
#undef STRICT
#undef DELETE

struct ConfTerm {
	HANDLE hIn;
	DWORD modeSave;
};

bool conf_term (ConfTerm& ct);
void unconf_term (ConfTerm& ct);
#else

#endif

ssize_t read_input (std::span<uint8_t> buf);
