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

	void feed (uint8_t c) noexcept;
	std::vector<Token> feed_string (std::string_view str);

	OptToken get () noexcept { return ctx.take_ready(); }

	void switch_mode (Mode mode) noexcept {
		ctx.mode = mode;
		reset();
	}

	OptToken flush () noexcept {
		fsm.event (FSMDetail::Events::EV_FLUSH);
		return get();
	}

	void reset () noexcept { fsm.event (FSMDetail::Events::EV_RESET); }

	Tokenizer (Mode mode = Mode::INPUT);
};

}
