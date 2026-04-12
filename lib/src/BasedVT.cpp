#include "BasedVT/BasedVT.hpp"

#include "Decode.hpp"

namespace BasedVT {

std::optional<KeyInput> Parser::decode (const Token& t) noexcept {
	return BasedVT::decode (t);
}

std::vector<KeyInput> Parser::parse_string (std::string_view str) {
	std::vector<KeyInput> keys;
	std::vector<Token> tokens = tokenizer.feed_string (str);
	for (const Token& token : tokens) {
		if (std::optional<KeyInput> key = decode (token))
			keys.emplace_back (std::move (*key));
	}
	return keys;
}

}
