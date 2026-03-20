#include "FSMCallbacks.hpp"

namespace BasedVT {

using namespace FSMDetail;
using ECResult = FSM::EventCallbackResult;

ECResult ev_esc_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	if (state == States::ST_ESC) return state;
	state = States::ST_ESC;
	fsm->switch_state (state);
	return state;
}

ECResult ev_execute_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	action (Actions::AC_EXECUTE, ctx);
	return state;
}

ECResult ev_printable_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_intermediate_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_digit_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_colon_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_semicolon_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_private_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_final_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();

	return state;
}

ECResult ev_delete_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_GROUND:
			action (Actions::AC_EXECUTE, ctx); [[fallthrough]]
		case States::ST_ESC:
		case States::ST_ESC_INTER:
			return state;
	}
	return state;
}

std::optional <Events> byte_to_event (char b, FSMDetail::States currState) {
	switch (b) {
		case 0x00 ... 0x1a:
		case 0x1c ... 0x1f: return Events::EV_EXECUTE;
		case 0x1b: return Events::EV_ESC;
		case 0x7f: return Events::EV_DELETE;
	};

	if (currState == States::ST_GROUND) [[likely]] {
		if (b >= 0x20 && b <= 0x7e) [[likely]]
			return Events::EV_PRINTABLE;
		return std::nullopt;
	}

	switch (b) {
		case 0x20 ... 0x2f: return Events::EV_INTERMEDIATE;
		case 0x30 ... 0x39: return Events::EV_DIGIT;
		case 0x3a: return Events::EV_COLON;
		case 0x3b: return Events::EV_SEMICOLON;
		case 0x3c ... 0x3f: return Events::EV_PRIVATE;
		case 0x40 ... 0x7e: return Events::EV_FINAL;
	};

	return std::nullopt;
}

}
