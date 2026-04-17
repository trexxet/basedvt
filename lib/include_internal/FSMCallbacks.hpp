#pragma once

#include "BasedVT/FSMDetail.hpp"
#include "FSMActions.hpp"

#include <optional>

namespace BasedVT {

using EventCallback = FSMDetail::FSM::EventCallback;

EventCallback ev_reset_cb;
EventCallback ev_flush_cb;
EventCallback ev_esc_cb;
EventCallback ev_execute_cb;
EventCallback ev_printable_cb;
EventCallback ev_intermediate_cb;
EventCallback ev_param_cb;
EventCallback ev_colon_cb;
EventCallback ev_private_cb;
EventCallback ev_final_cb;
EventCallback ev_csi_entry_cb;
EventCallback ev_ss3_entry_cb;

std::optional <FSMDetail::Events> byte_to_event (uint8_t b, FSMDetail::Mode mode, FSMDetail::States state);

consteval auto make_callbacks () {
	using namespace FSMDetail;
	constexpr auto do_clear = [] (Context* ctx) { action (Actions::AC_CLEAR, ctx); };
	return FSM::make_callbacks (
		FSM::event_cb <Events::EV_RESET>        (ev_reset_cb),
		FSM::event_cb <Events::EV_FLUSH>        (ev_flush_cb),
		FSM::event_cb <Events::EV_ESC>          (ev_esc_cb),
		FSM::event_cb <Events::EV_EXECUTE>      (ev_execute_cb),
		FSM::event_cb <Events::EV_PRINTABLE>    (ev_printable_cb),
		FSM::event_cb <Events::EV_INTERMEDIATE> (ev_intermediate_cb),
		FSM::event_cb <Events::EV_PARAM>        (ev_param_cb),
		FSM::event_cb <Events::EV_COLON>        (ev_colon_cb),
		FSM::event_cb <Events::EV_PRIVATE>      (ev_private_cb),
		FSM::event_cb <Events::EV_FINAL>        (ev_final_cb),
		FSM::event_cb <Events::EV_CSI_ENTRY>    (ev_csi_entry_cb),
		FSM::event_cb <Events::EV_SS3_ENTRY>    (ev_ss3_entry_cb),
		FSM::state_cb <States::ST_ESC>          ({.on_enter = do_clear}),
		FSM::state_cb <States::ST_CSI_ENTRY>    ({.on_enter = do_clear})
	);
}

}
