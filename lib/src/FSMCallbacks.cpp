#include "FSMCallbacks.hpp"

namespace BasedVT {

using namespace FSMDetail;
using ECResult = FSM::EventCallbackResult;

ECResult ev_esc_cb (FSM* fsm, Context* ctx) {
	States state = States::ST_ESC;
	fsm->switch_state (state);
	return state;
}

ECResult ev_execute_cb (FSM* fsm, Context* ctx) {
	action (Actions::AC_EXECUTE, ctx);
	return fsm->state();
}

ECResult ev_printable_cb (FSM* fsm, Context* ctx) {
	action (Actions::AC_PRINT, ctx);
	return fsm->state();
}

ECResult ev_intermediate_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
		case States::ST_ESC_INTER:
			action (Actions::AC_COLLECT, ctx);
			state = States::ST_ESC_INTER;
			break;
		case States::ST_CSI_ENTRY:
		case States::ST_CSI_PARAM:
		case States::ST_CSI_INTER:
			action (Actions::AC_COLLECT, ctx);
			state = States::ST_CSI_INTER;
			break;
	}
	fsm->switch_state (state);
	return state;
}

ECResult ev_digit_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
		case States::ST_ESC_INTER:
			action (Actions::AC_ESC_DISPATCH, ctx);
			state = States::ST_GROUND;
			break;
		case States::ST_CSI_ENTRY:
		case States::ST_CSI_PARAM:
			action (Actions::AC_PARAM, ctx);
			state = States::ST_CSI_PARAM;
			break;
		case States::ST_CSI_INTER:
		case States::ST_CSI_IGNORE:
			state = States::ST_CSI_IGNORE;
			break;
	}
	fsm->switch_state (state);
	return state;
}

ECResult ev_colon_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
		case States::ST_ESC_INTER:
			action (Actions::AC_ESC_DISPATCH, ctx);
			state = States::ST_GROUND;
			break;
		case States::ST_CSI_ENTRY:
		case States::ST_CSI_PARAM:
		case States::ST_CSI_INTER:
		case States::ST_CSI_IGNORE:
			state = States::ST_CSI_IGNORE;
			break;
	}
	fsm->switch_state (state);
	return state;
}

// TODO: same as digit?
ECResult ev_semicolon_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
		case States::ST_ESC_INTER:
			action (Actions::AC_ESC_DISPATCH, ctx);
			state = States::ST_GROUND;
			break;
		case States::ST_CSI_ENTRY:
		case States::ST_CSI_PARAM:
			action (Actions::AC_PARAM, ctx);
			state = States::ST_CSI_PARAM;
			break;
		case States::ST_CSI_INTER:
		case States::ST_CSI_IGNORE:
			state = States::ST_CSI_IGNORE;
			break;
	}
	fsm->switch_state (state);
	return state;
}

ECResult ev_private_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
		case States::ST_ESC_INTER:
			action (Actions::AC_ESC_DISPATCH, ctx);
			state = States::ST_GROUND;
			break;
		case States::ST_CSI_ENTRY:
			action (Actions::AC_COLLECT, ctx);
			state = States::ST_CSI_PARAM;
			break;
		case States::ST_CSI_PARAM:
		case States::ST_CSI_INTER:
		case States::ST_CSI_IGNORE:
			state = States::ST_CSI_IGNORE;
			break;
	}
	fsm->switch_state (state);
	return state;
}

ECResult ev_csi_entry_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
			state = States::ST_CSI_ENTRY;
			break;
		default: break;
	}
	fsm->switch_state (state);
	return state;
}

ECResult ev_final_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
		case States::ST_ESC_INTER:
			action (Actions::AC_ESC_DISPATCH, ctx);
			state = States::ST_GROUND;
			break;
		case States::ST_CSI_ENTRY:
		case States::ST_CSI_PARAM:
		case States::ST_CSI_INTER:
			action (Actions::AC_CSI_DISPATCH, ctx); [[fallthrough]];
		case States::ST_CSI_IGNORE:
			state = States::ST_GROUND;
			break;
	}
	fsm->switch_state (state);
	return state;
}

ECResult ev_delete_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	if (state == States::ST_GROUND)
		action (Actions::AC_PRINT, ctx);
	return state;
}

std::optional <Events> byte_to_event (uint8_t b, FSMDetail::States currState) {
	switch (b) {
		case 0x00 ... 0x1A:
		case 0x1C ... 0x1F: return Events::EV_EXECUTE;
		case 0x1B: return Events::EV_ESC;
		case 0x7F: return Events::EV_DELETE;
	};

	if (currState == States::ST_GROUND) [[likely]] {
		if (b >= 0x20 && b <= 0x7E) [[likely]]
			return Events::EV_PRINTABLE;
		return std::nullopt;
	}

	if (currState == States::ST_ESC) {
		if (b == 0x5B)
			return Events::EV_CSI_ENTRY;
	}

	switch (b) {
		case 0x20 ... 0x2F: return Events::EV_INTERMEDIATE;
		case 0x30 ... 0x39: return Events::EV_DIGIT;
		case 0x3A: return Events::EV_COLON;
		case 0x3B: return Events::EV_SEMICOLON;
		case 0x3C ... 0x3F: return Events::EV_PRIVATE;
		case 0x40 ... 0x7E: return Events::EV_FINAL;
	};

	return std::nullopt;
}

}
