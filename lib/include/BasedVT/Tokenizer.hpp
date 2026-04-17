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
	FSMDetail::Mode mode;
public:
	using Mode = FSMDetail::Mode;

	void feed (uint8_t c);
	std::vector<Token> feed_string (std::string_view str);

	OptToken get () noexcept {
		if (ctx.ready) {
			Token token = ctx.token;
			reset();
			return token;
		}
		else return std::nullopt;
	}

	void switch_mode (Mode mode) noexcept { this->mode = mode; }

	OptToken flush () noexcept {
		fsm.event (FSMDetail::Events::EV_FLUSH);
		return get();
	}

	void reset () noexcept { fsm.event (FSMDetail::Events::EV_RESET); }

	Tokenizer (Mode mode);
};

}
