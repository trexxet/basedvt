#pragma once

#include <cstdint>
#include <string_view>
#include <optional>
#include <vector>

#include "Key.hpp"
#include "Tokenizer.hpp"

namespace BasedVT {

class Parser {
	Tokenizer tokenizer;
public:
	void feed (uint8_t c) { tokenizer.feed (c); }

	static std::optional<KeyInput> decode (const Token& t) noexcept;

	std::optional<KeyInput> get () noexcept {
		return tokenizer.get().and_then (decode);
	}

	std::vector<KeyInput> parse_string (std::string_view str);
};


}
