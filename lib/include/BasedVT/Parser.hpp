#pragma once

#include <cstdint>
#include <string_view>
#include <optional>
#include <vector>

#include "BasedVT/Decode.hpp"
#include "BasedVT/Tokenizer.hpp"

#include "BasedVT/InputEvent/Event.hpp"
#include "BasedVT/InputEvent/Key.hpp"

namespace BasedVT {

class Parser {
	Tokenizer tokenizer;
public:
	void feed (uint8_t c) { tokenizer.feed (c); }

	InputEvent::OptKeyInput get () noexcept {
		return tokenizer.get().and_then (decode);
	}

	InputEvent::OptKeyInput flush () noexcept {
		return tokenizer.flush().and_then (decode);
	}

	std::vector<InputEvent::KeyInput> parse_string (std::string_view str) {
		std::vector<InputEvent::KeyInput> keys;
		std::vector<Token> tokens = tokenizer.feed_string (str);
		for (const Token& token : tokens) {
			if (InputEvent::OptKeyInput key = decode (token))
				keys.emplace_back (std::move (*key));
		}
		return keys;
	}

	void switch_mode (Tokenizer::Mode mode) noexcept { tokenizer.switch_mode (mode); }

	Parser (Tokenizer::Mode mode = Tokenizer::Mode::INPUT) : tokenizer (mode) { }
};


}
