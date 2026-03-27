#include "BasedVT/BasedVT.hpp"

#include "Decode.hpp"
#include "FSMCallbacks.hpp"

#include <optional>

namespace BasedVT {

using namespace FSMDetail;

void Parser::feed (uint8_t c) {
	std::optional <Events> ev = byte_to_event (c, fsm.state());
	if (ev) {
		ctx.currByte = c;
		fsm.event (ev.value());
	}
}

std::optional<KeyInput> Parser::get_decode (const Token& t) const noexcept {
	return decode (t);
}

Parser::Parser () : fsm (States::ST_GROUND, &ctx, make_callbacks ()) {

}

}
