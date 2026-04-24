#include "demoCommon.hpp"

#ifdef __WIN32
ConfTerm::ConfTerm () {
	hIn = GetStdHandle (STD_INPUT_HANDLE);
	GetConsoleMode (hIn, &modeSave);
	DWORD mode = modeSave | ENABLE_VIRTUAL_TERMINAL_INPUT;
	mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
	if (!SetConsoleMode (hIn, mode)) {
		std::print ("SetConsoleMode failed\n");
		return;
	}
	ok = true;
}

ConfTerm::~ConfTerm () {
	SetConsoleMode (hIn, modeSave);
}
#else
ConfTerm::ConfTerm () {
	if (!isatty (STDIN_FILENO)) {
		std::print (stderr, "stdin is not a tty\n");
		return;
	}
	if (tcgetattr (STDIN_FILENO, &save) != 0) {
		std::perror ("tcgetattr");
		return;
	}

	saved = true;
	termios cfg = save;
	cfg.c_lflag &= ~(ICANON | ISIG | ECHO);
	cfg.c_iflag &= ~(IXON);
	cfg.c_cc[VMIN] = 1;
	cfg.c_cc[VTIME] = 0;

	if (tcsetattr (STDIN_FILENO, TCSAFLUSH, &cfg) != 0) {
		std::perror ("tcsetattr");
		return;
	}
	ok = true;
}

ConfTerm::~ConfTerm () {
	if (!saved) return;
	if (tcsetattr (STDIN_FILENO, TCSANOW, &save) != 0)
		std::perror ("tcsetattr");
}
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
