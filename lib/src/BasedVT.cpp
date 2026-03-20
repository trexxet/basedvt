#include "BasedVT/BasedVT.hpp"

#include "FSMCallbacks.hpp"

#include <optional>

namespace BasedVT {

using namespace FSMDetail;

void Parser::feed (char c) {
	std::optional <Events> ev = byte_to_event (c, fsm.state());
	if (ev)
		fsm.event (ev.value());
}

Parser::Parser () : fsm (States::ST_GROUND, &ctx, make_callbacks ()) {

}

}
