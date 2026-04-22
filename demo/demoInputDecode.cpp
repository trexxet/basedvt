#include "BasedVT/Parser.hpp"

#include <cstdio>
#include <cstdint>
#include <format>
#include <print>
#include <span>
#include <string>
#include <variant>

#include "Basedlib/Core/FSM.hpp"
#include "Basedlib/Meta/Overloaded.hpp"
#include "Basedlib/Meta/PrettyEnum.hpp"

// TODO: linux

#ifdef __WIN32
#include <windows.h>
#include <io.h>
// bruh
#undef STRICT

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
#else

#endif

ssize_t read_input (std::span<uint8_t> buf) {
	ssize_t n = read (STDIN_FILENO, buf.data(), buf.size());
	std::print ("Read: {}\n", n);

	for (ssize_t i = 0; i < n; i++) {
		std::print ("0x{:02x} ", static_cast<int>(buf[i]));
	}
	std::putchar ('\n');

	return n;
}

void print_decoded_key (const BasedVT::InputEvent::KeyInput& key) {
	std::string str = std::format ("Done: {}", BasedVT::InputEvent::KeyInput::PrettyKey::to_string (key.key));
	if (key.key == BasedVT::InputEvent::KeyInput::Key::CHAR)
		str.append (std::isspace (key.byte) ? " whitespace" : std::format(" {}", static_cast<char> (key.byte)));
	if (key.ctrl) str.append (" ctrl");
	if (key.alt) str.append (" alt");
	if (key.shift) str.append (" shift");
	std::print ("{}\n", str);
}

bool needs_flush (std::span<uint8_t> buf, uint8_t currByte) {
	return (buf.size() == 1 && buf[0] == '\e') // single ESC
	    || (buf.size() == 2 && buf[0] == '\e' && currByte == '[')  // CSI entry
	    || (buf.size() == 2 && buf[0] == '\e' && currByte == 'O'); // SS3 entry
}

void decode (BasedVT::Parser& parser, std::span<uint8_t> buf) {
	bool decodedAny = false;

	for (uint8_t b : buf) {
		parser.feed (b);
		auto event = needs_flush (buf, b) ? parser.flush() : parser.get();
		if (!event) continue;
		std::visit (Basedlib::Overloaded {
			[] (BasedVT::InputEvent::KeyInput k) { print_decoded_key (k); }
		}, *event);
		decodedAny = true;
	}

	if (!decodedAny)
		std::print ("No key decoded\n");
}

int main () {
#ifdef __WIN32
	ConfTerm ct;
	if (!conf_term (ct)) return 1;
#endif
	std::print ("BasedVT Input Decode Demo\n");

	uint8_t buf[64];
	ssize_t n = read_input (buf);

	BasedVT::Parser parser;
	decode (parser, std::span (buf, n));

#ifdef __WIN32
	unconf_term (ct);
#endif
	return 0;
}
