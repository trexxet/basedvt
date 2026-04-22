#include "demoCommon.hpp"
#include "BasedVT/Tokenizer.hpp"

#include <cstring>

static_assert (BASEDVT_ENABLE_TO_STRING);

bool needs_flush (std::span<uint8_t> buf, uint8_t currByte) {
	return (buf.size() == 1 && buf[0] == '\e'); // single ESC
}

void tokenize (BasedVT::Tokenizer& tokenizer, std::span<uint8_t> buf) {
	bool tokenizedAny = false;

	for (uint8_t b : buf) {
		tokenizer.feed (b);
		auto token = needs_flush (buf, b) ? tokenizer.flush() : tokenizer.get();
		if (!token) continue;
		std::print ("Token: {}\n", token->to_string());
		tokenizedAny = true;
	}

	if (!tokenizedAny)
		std::print ("No token obtained\n");
}

int main (int argc, char* argv[]) {
#ifdef __WIN32
	ConfTerm ct;
	if (!conf_term (ct)) return 1;
#endif
	std::print ("BasedVT Tokenizer Strict mode demo\n");

	uint8_t buf[64];
	ssize_t n = 0;

	if (argc == 1) {
		std::print ("No input arg - reading key input\n");
		n = read_input (buf);
	} else if (argc == 2 && argv[1]) {
		std::print ("Reading argv[1]\n");
		n = std::min (std::strlen (argv[1]), sizeof(buf));
		std::memcpy (buf, argv[1], n);
	} else {
		std::print ("Bad input\n");
	}

	BasedVT::Tokenizer tokenizer (BasedVT::Tokenizer::Mode::STRICT);
	tokenize (tokenizer, std::span (buf, n));

#ifdef __WIN32
	unconf_term (ct);
#endif
	return 0;
}
