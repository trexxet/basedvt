#pragma once

#include "Basedlib/FSM.hpp"

#include <string>

#ifdef BASEDVT_DEBUG
#include <print>
#include <string_view>
#endif

namespace BasedVT::FSMDetail {

struct Context {
	std::string input;
};

enum class States {
	ST_GROUND,
	ST_ESC,
	ST_ESC_INTER,
	ST_CSI_ENTRY,
	ST_CSI_PARAM,
	ST_CSI_INTER,
	ST_CSI_IGNORE
};

enum class Events {
	EV_PRINTABLE,
	EV_DIGIT,
	EV_SEMICOLON,
	EV_ESC
};

using FSM = Basedlib::FSM::FSM <Basedlib::FSM::Enum<States>, Basedlib::FSM::Enum<Events>, Context
#ifdef BASEDVT_DEBUG
	, [] (std::string_view s) { std::print ("FSM Logger: {}\n", s); }
#endif
>;

};
