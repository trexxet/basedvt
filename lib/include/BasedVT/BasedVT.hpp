#pragma once

#include <cstdint>
#include <optional>

#include "FSMDetail.hpp"
#include "Key.hpp"
#include "Token.hpp"

namespace BasedVT {

class Parser {

	FSMDetail::Context ctx;
	FSMDetail::FSM fsm;

public:
	void feed (uint8_t c);

	std::optional<Token> get () const noexcept {
		if (ctx.ready) return ctx.token;
		else return std::nullopt;
	}

	std::optional<KeyInput> get_decode (const Token& t) const noexcept;

	Parser ();
};


}
