#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "FSMDetail.hpp"
#include "Token.hpp"

namespace BasedVT {

class Tokenizer {
	FSMDetail::Context ctx;
	FSMDetail::FSM fsm;
public:
	void feed (uint8_t c);
	std::vector<Token> feed_string (std::string_view str);

	OptToken get () noexcept {
		if (ctx.ready) {
			Token token = ctx.token;
			ctx.token = {};
			return token;
		}
		else return std::nullopt;
	}

	void reset () noexcept;

	Tokenizer ();
};

}
