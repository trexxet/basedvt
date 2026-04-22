#include "demoCommon.hpp"
#include "BasedVT/Tokenizer.hpp"

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

int main () {
#ifdef __WIN32
	ConfTerm ct;
	if (!conf_term (ct)) return 1;
#endif
	std::print ("BasedVT Tokenizer Strict mode demo\n");

	uint8_t buf[64];
	ssize_t n = read_input (buf);

	BasedVT::Tokenizer tokenizer (BasedVT::Tokenizer::Mode::STRICT);
	tokenize (tokenizer, std::span (buf, n));

#ifdef __WIN32
	unconf_term (ct);
#endif
	return 0;
}
