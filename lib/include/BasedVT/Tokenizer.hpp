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
	using Mode = FSMDetail::Mode;

	void feed (uint8_t c);
	std::vector<Token> feed_string (std::string_view str);

	OptToken get () noexcept {
		if (ctx.ready) {
			Token token = ctx.token;
			ctx.reset();
			return token;
		}
		else return std::nullopt;
	}

	void switch_mode (Mode mode) noexcept { ctx.mode = mode; }
	OptToken flush () noexcept;
	void reset () noexcept;
	void reset (Mode mode) noexcept {
		switch_mode (mode);
		reset();
	}

	Tokenizer (Mode mode);
};

}
