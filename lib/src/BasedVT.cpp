#include "BasedVT/BasedVT.hpp"

#include "FSMCallbacks.hpp"

namespace BasedVT {

using namespace FSMDetail;

void Parser::feed (char c) {
	fsm.event (process_input_byte (c));
}

Parser::Parser () : fsm (States::ST_GROUND, &ctx, make_callbacks ()) {

}

}
