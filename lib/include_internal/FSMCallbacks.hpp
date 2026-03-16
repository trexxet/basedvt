#pragma once

#include "BasedVT/FSMDetail.hpp"

namespace BasedVT {

using EventCallback = FSMDetail::FSM::EventCallback;

EventCallback ev_printable_cb;
EventCallback ev_digit_cb;
EventCallback ev_semicolon_cb;
EventCallback ev_esc_cb;

FSMDetail::Events byte_to_event (char b, FSMDetail::States currState);

consteval auto make_callbacks() {
	using namespace FSMDetail;
	return FSM::make_callbacks (
		FSM::event_cb <Events::EV_PRINTABLE> ({ev_printable_cb}),
		FSM::event_cb <Events::EV_DIGIT> ({ev_digit_cb}),
		FSM::event_cb <Events::EV_SEMICOLON> ({ev_semicolon_cb}),
		FSM::event_cb <Events::EV_ESC> ({ev_esc_cb})
	);
}

}
