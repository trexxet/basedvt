#include "FSMCallbacks.hpp"

namespace BasedVT {

using namespace FSMDetail;
using ECResult = FSM::EventCallbackResult;

ECResult ev_reset_cb (FSM* fsm, Context* ctx) {
	action (Actions::AC_CLEAR, ctx);
	States state = States::ST_GROUND;
	return fsm->switch_state (state);
}

ECResult ev_flush_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
			action (Actions::AC_CLEAR, ctx);
			action (Actions::AC_ESC_DISPATCH, ctx);
			break;
		default:
			action (Actions::AC_CLEAR, ctx);
			break;
	}
	state = States::ST_GROUND;
	return fsm->switch_state (state);
}

ECResult ev_esc_cb (FSM* fsm, Context* ctx) {
	States state = States::ST_ESC;
	return fsm->switch_state (state);
}

ECResult ev_execute_cb (FSM* fsm, Context* ctx) {
	action (Actions::AC_EXECUTE, ctx);
	return fsm->state();
}

ECResult ev_printable_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_GROUND:
			action (Actions::AC_PRINT, ctx);
			break;
		case States::ST_SS3:
			action (Actions::AC_SS3_DISPATCH, ctx);
			state = States::ST_GROUND;
			break;
	}
	return fsm->switch_state (state);
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
	return fsm->switch_state (state);
}

ECResult ev_param_cb (FSM* fsm, Context* ctx) {
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
	return fsm->switch_state (state);
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
	return fsm->switch_state (state);
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
	return fsm->switch_state (state);
}

ECResult ev_csi_entry_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
			state = States::ST_CSI_ENTRY;
			break;
	}
	return fsm->switch_state (state);
}

ECResult ev_ss3_entry_cb (FSM* fsm, Context* ctx) {
	States state = fsm->state();
	switch (state) {
		case States::ST_ESC:
			state = States::ST_SS3;
			break;
	}
	return fsm->switch_state (state);
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
	return fsm->switch_state (state);
}

std::optional <Events> byte_to_event (uint8_t b, FSMDetail::States currState) {
	switch (b) {
		case 0x00 ... 0x1A:
		case 0x1C ... 0x1F: return Events::EV_EXECUTE;
		case 0x1B: return Events::EV_ESC;
	};

	if (currState == States::ST_GROUND) {
		if (b == 0x7F) return Events::EV_EXECUTE;
	}

	if (currState == States::ST_GROUND || currState == States::ST_SS3) [[likely]] {
		if (b >= 0x20 && b <= 0x7E) [[likely]]
			return Events::EV_PRINTABLE;
		return std::nullopt;
	}

	if (currState == States::ST_ESC) {
		if (b == 0x5B) return Events::EV_CSI_ENTRY;
		if (b == 0x4F) return Events::EV_SS3_ENTRY;
	}

	switch (b) {
		case 0x20 ... 0x2F: return Events::EV_INTERMEDIATE;
		case 0x30 ... 0x39:
		case 0x3B: return Events::EV_PARAM;
		case 0x3A: return Events::EV_COLON;
		case 0x3C ... 0x3F: return Events::EV_PRIVATE;
		case 0x40 ... 0x7E: return Events::EV_FINAL;
	};

	return std::nullopt;
}

}
