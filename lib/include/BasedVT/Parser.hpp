#pragma once

#include <cstdint>
#include <string_view>
#include <optional>
#include <vector>

#include "Decode.hpp"
#include "Key.hpp"
#include "Tokenizer.hpp"

namespace BasedVT {

class Parser {
	Tokenizer tokenizer;
public:
	void feed (uint8_t c) { tokenizer.feed (c); }

	OptKeyInput get () noexcept {
		return tokenizer.get().and_then (decode);
	}

	OptKeyInput flush () noexcept {
		return tokenizer.flush().and_then (decode);
	}

	std::vector<KeyInput> parse_string (std::string_view str) {
		std::vector<KeyInput> keys;
		std::vector<Token> tokens = tokenizer.feed_string (str);
		for (const Token& token : tokens) {
			if (OptKeyInput key = decode (token))
				keys.emplace_back (std::move (*key));
		}
		return keys;
	}

	void switch_mode (Tokenizer::Mode mode) noexcept { tokenizer.switch_mode (mode); }

	Parser (Tokenizer::Mode mode = Tokenizer::Mode::INPUT) : tokenizer (mode) { }
};


}
