#include "BasedVT/BasedVT.hpp"

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

Parser::Parser () : fsm (States::ST_GROUND, &ctx, make_callbacks ()) {

}

}
