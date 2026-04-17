#include "BasedVT/Parser.hpp"

#include <cstdio>
#include <cstdint>
#include <format>
#include <print>
#include <span>
#include <string>

#include <windows.h>

#include <io.h>

#include "Basedlib/Core/FSM.hpp"

// TODO: linux

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

ssize_t read_input (std::span<uint8_t> buf) {
	ssize_t n = read (STDIN_FILENO, buf.data(), buf.size());
	std::print ("Read: {}\n", n);

	for (ssize_t i = 0; i < n; i++) {
		std::print ("0x{:02x} ", static_cast<int>(buf[i]));
	}
	std::putchar ('\n');

	return n;
}

int main () {
	ConfTerm ct;
	if (!conf_term (ct)) return 0;
	std::print ("BasedVT Demo\n");

	uint8_t buf[64];
	ssize_t n = read_input (buf);

	BasedVT::Parser vtParser;

	bool decodedAny = false;
	for (ssize_t i = 0; i < n; i++) {
		vtParser.feed (buf[i]);
		auto key = vtParser.get();
		if (key) {
			std::string str = std::format ("Done: {}", BasedVT::KeyInput::PrettyKey::to_string (key->key));
			if (key->key == BasedVT::KeyInput::Key::CHAR)
				str.append (std::isspace (key->ch) ? " whitespace" : std::format(" {}", key->ch));
			if (key->ctrl) str.append (" ctrl");
			if (key->alt) str.append (" alt");
			if (key->shift) str.append (" shift");
			std::print ("{}\n", str);
			decodedAny = true;
		}
	}
	if (!decodedAny)
		std::print ("No key decoded\n");

	unconf_term (ct);
	return 0;
}
