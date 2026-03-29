#include "Basedlib/FSM.hpp"
#include "BasedVT/BasedVT.hpp"

#include <cstdio>
#include <cstdint>
#include <print>

#include <windows.h>

#include <io.h>

struct ConfTerm {
	HANDLE hIn;
	DWORD modeSave;
};

bool conf_term (ConfTerm& ct) {
	ct.hIn = GetStdHandle (STD_INPUT_HANDLE);
	GetConsoleMode (ct.hIn, &ct.modeSave);
	DWORD mode = ct.modeSave | ENABLE_VIRTUAL_TERMINAL_INPUT;
	mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
	if (!SetConsoleMode (ct.hIn, mode)) {
		std::print ("SetConsoleMode failed\n");
		return false;
	}
	return true;
}

void unconf_term (ConfTerm& ct) {
	SetConsoleMode (ct.hIn, ct.modeSave);
}

int main () {
	ConfTerm ct;
	if (!conf_term (ct)) return 0;
	std::print ("VT Test\n");

	uint8_t buf[64];
	ssize_t n = read (STDIN_FILENO, buf, sizeof(buf));
	std::print ("Read: {}\n", n);

	for (ssize_t i = 0; i < n; i++) {
		std::print ("0x{:02x} ", static_cast<int>(buf[i]));
	}
	std::putchar ('\n');

	BasedVT::Parser vtParser;

	for (ssize_t i = 0; i < n; i++) {
		vtParser.feed (buf[i]);
		auto token = vtParser.get();
		if (token)
			auto key = vtParser.get_decode (token.value());
	}

	unconf_term (ct);
	return 0;
}
