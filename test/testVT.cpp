#include "Basedlib/FSM.hpp"

#include <cstdio>
#include <print>

#include <windows.h>

#include <io.h>

struct ConfTerm {
	HANDLE hIn;
	DWORD modeSave;
};

bool conf_term (ConfTerm& ct) {
	ct.hIn = GetStdHandle (STD_INPUT_HANDLE);
	GetConsoleMode (ct.hIn, &ct.modeSave);
	DWORD mode = ct.modeSave | ENABLE_VIRTUAL_TERMINAL_INPUT;
	if (!SetConsoleMode (ct.hIn, mode)) {
		std::print ("SetConsoleMode failed\n");
		return false;
	}
	return true;
}

void unconf_term (ConfTerm& ct) {
	SetConsoleMode (ct.hIn, ct.modeSave);
}

struct VtContext {
	std::string input;
};

enum class VtStates {
	ST_NORMAL,
	ST_ESC,
	ST_CSI,
	ST_CSI_PARAM
};

enum class VtEvents {
	EV_PRINTABLE,
	EV_DIGIT,
	EV_SEMICOLON,
	EV_ESC
};

using VtFSM = Basedlib::FSM::FSM <Basedlib::FSM::Enum<VtStates>, Basedlib::FSM::Enum<VtEvents>, VtContext,
	[] (std::string_view s) { std::print ("FSM Logger: {}\n", s); }
>;

VtFSM::EventCallbackResult ev_printable_cb (VtFSM* fsm, VtContext* ctx) {
	VtStates state = fsm->state();

	return state;
}

VtFSM::EventCallbackResult ev_digit_cb (VtFSM* fsm, VtContext* ctx) {
	VtStates state = fsm->state();

	return state;
}

VtFSM::EventCallbackResult ev_semicolon_cb (VtFSM* fsm, VtContext* ctx) {
	VtStates state = fsm->state();

	return state;
}

VtFSM::EventCallbackResult ev_esc_cb (VtFSM* fsm, VtContext* ctx) {
	VtStates state = fsm->state();
	switch (state) {
		case VtStates::ST_NORMAL: state = VtStates::ST_ESC; break;
		default: return VtFSM::EventNotPermitted;
	}
	fsm->switch_state (state);
	return state;
}

VtEvents process_input_byte (char b) {
	VtEvents ev = VtEvents::EV_PRINTABLE;
	switch (b) {
		case '0'...'9': ev = VtEvents::EV_DIGIT; break;
		case 0x1b: ev = VtEvents::EV_ESC; break;
		case ';': ev = VtEvents::EV_SEMICOLON; break;
		default: ev = VtEvents::EV_PRINTABLE; break;
	};
	return ev;
}

int main () {
	ConfTerm ct;
	if (!conf_term (ct)) return 0;
	std::print ("VT Test\n");

	VtContext ctx;
	VtFSM fsm (VtStates::ST_NORMAL, &ctx, VtFSM::make_callbacks(
		VtFSM::event_cb <VtEvents::EV_PRINTABLE> ({ev_printable_cb}),
		VtFSM::event_cb <VtEvents::EV_DIGIT> ({ev_digit_cb}),
		VtFSM::event_cb <VtEvents::EV_SEMICOLON> ({ev_semicolon_cb}),
		VtFSM::event_cb <VtEvents::EV_ESC> ({ev_esc_cb})
	));

	char buf[64];
	ssize_t n = read (STDIN_FILENO, buf, sizeof(buf));
	std::print ("Read: {}\n", n);

	for (ssize_t i = 0; i < n; i++) {
		std::print ("0x{:02x} ", static_cast<int>(buf[i]));
	}
	std::putchar ('\n');

	for (ssize_t i = 0; i < n; i++) {
		fsm.event (process_input_byte (buf[i]));
	}

	unconf_term (ct);
	return 0;
}
