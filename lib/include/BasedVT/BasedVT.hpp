#pragma once

#include <cstdint>
#include <optional>

#include "FSMDetail.hpp"

namespace BasedVT {

class Parser {

	FSMDetail::Context ctx;
	FSMDetail::FSM fsm;

public:
	void feed (uint8_t c);

	std::optional <FSMDetail::Token> get () const noexcept {
		if (ctx.ready) return ctx.token;
		else return std::nullopt;
	}

	Parser ();
};


}
