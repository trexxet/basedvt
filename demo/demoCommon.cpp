#include "demoCommon.hpp"

#ifdef __WIN32
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
