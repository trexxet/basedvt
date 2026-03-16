#include "BasedVT/BasedVT.hpp"

#include "FSMCallbacks.hpp"

namespace BasedVT {

using namespace FSMDetail;

void Parser::feed (char c) {
	fsm.event (byte_to_event (c, fsm.state()));
}

Parser::Parser () : fsm (States::ST_GROUND, &ctx, make_callbacks ()) {

}

}
