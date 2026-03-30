#include "BasedVT/BasedVT.hpp"

#include "Decode.hpp"
#include "FSMCallbacks.hpp"

#include <optional>

namespace BasedVT {

using namespace FSMDetail;

void Parser::feed (uint8_t c) {
	if (std::optional<Events> ev = byte_to_event (c, fsm.state())) {
		ctx.currByte = c;
		fsm.event (ev.value());
	}
}

std::optional<KeyInput> Parser::decode (const Token& t) noexcept {
	return BasedVT::decode (t);
}

std::vector<KeyInput> Parser::parse_string (std::string_view str) {
	std::vector<KeyInput> keys;
	for (char c : str) {
		feed (c);
		if (std::optional<KeyInput> key = get())
			keys.emplace_back (std::move (*key));
	}
	return keys;
}

void Parser::reset () noexcept {
	ctx = {};
	fsm.switch_state (States::ST_GROUND);
}

Parser::Parser () : fsm (States::ST_GROUND, &ctx, make_callbacks ()) { }

}
