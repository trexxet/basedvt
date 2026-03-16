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
	EV_ESC,          // 0x1B
	EV_EXECUTE,      // 0x00-0x17, 0x19, 0x1C-0x1F
	EV_PRINTABLE,    // 0x20-0x7F in ST_GROUND
	EV_INTERMEDIATE, // 0x20-0x2F
	EV_DIGIT,        // 0x30-0x39
	EV_COLON,        // 0x3A
	EV_SEMICOLON,    // 0x3B
	EV_PRIVATE,      // 0x3C-0x3F
	EV_FINAL,        // 0x40-0x7E
	EV_DELETE        // 0x7F
};

using FSM = Basedlib::FSM::FSM <Basedlib::FSM::Enum<States>, Basedlib::FSM::Enum<Events>, Context
#ifdef BASEDVT_DEBUG
	, [] (std::string_view s) { std::print ("FSM Logger: {}\n", s); }
#endif
>;

enum class Actions {
	AC_IGNORE,
	AC_PRINT,
	AC_EXECUTE,
	AC_CLEAR,
	AC_COLLECT,
	AC_PARAM,
	AC_ESC_DISPATCH,
	AC_CSI_DISPATCH
};

};
