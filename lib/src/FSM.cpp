#include "FSMCallbacks.hpp"

namespace BasedVT {

using namespace FSMDetail;
using ECResult = FSM::EventCallbackResult;

ECResult ev_printable_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_digit_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_semicolon_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_esc_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_GROUND: state = States::ST_ESC; break;
		default: return FSM::EventNotPermitted;
	}
	fsm->switch_state (state);
	return state;
}

Events process_input_byte (char b) {
	Events ev = Events::EV_PRINTABLE;
	switch (b) {
		case '0'...'9': ev = Events::EV_DIGIT; break;
		case 0x1b: ev = Events::EV_ESC; break;
		case ';': ev = Events::EV_SEMICOLON; break;
		default: ev = Events::EV_PRINTABLE; break;
	};
	return ev;
}

}
