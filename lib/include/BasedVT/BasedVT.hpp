#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

#include "FSMDetail.hpp"
#include "Key.hpp"
#include "Token.hpp"

namespace BasedVT {

class Parser {
	FSMDetail::Context ctx;
	FSMDetail::FSM fsm;

public:
	void feed (uint8_t c);

	std::optional<Token> get_token () const noexcept {
		if (ctx.ready) return ctx.token;
		else return std::nullopt;
	}

	static std::optional<KeyInput> decode (const Token& t) noexcept;

	std::optional<KeyInput> get () const noexcept {
		return get_token().and_then (decode);
	}

	std::vector<KeyInput> parse_string (std::string_view str);

	void reset () noexcept;

	Parser ();
};


}
