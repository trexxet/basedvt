#pragma once

#include <cstdint>
#include <optional>

#include "Basedlib/Core/FSM.hpp"
#include "Token.hpp"

#ifdef BASEDVT_DEBUG
#include <print>
#include <string_view>
#endif

namespace BasedVT::FSMDetail {

enum class Mode {
	STRICT,
	INPUT
};

struct Context {
	Mode mode;
	TokenStage stage;
	OptToken ready;

	OptToken take_ready () noexcept { return std::exchange (ready, std::nullopt); }
};

enum class States {
	ST_GROUND,
	ST_ESC,
	ST_ESC_INTER,
	ST_CSI_ENTRY,
	ST_CSI_PARAM,
	ST_CSI_INTER,
	ST_CSI_IGNORE,
	ST_SS3
};

enum class Events {
	EV_RESET,          // Reset ctx and go to ST_GROUND
	EV_FLUSH,          // Force an end of the sequence and try to tokenize whatever it got
	EV_ESC,            // 0x1B
	EV_EXECUTE,        // 0x00-0x17, 0x19, 0x1C-0x1F; 0x7F in ST_GROUND
	EV_EXECUTE_CANCEL, // 0x18, 0x1A
	EV_PRINTABLE,      // 0x20-0x7F in ST_GROUND or ST_SS3
	EV_INTERMEDIATE,   // 0x20-0x2F
	EV_PARAM,          // 0x30-0x39, 0x3B ';'
	EV_COLON,          // 0x3A ':'
	EV_PRIVATE,        // 0x3C-0x3F
	EV_FINAL,          // 0x40-0x7E
	EV_CSI_ENTRY,      // 0x5B '[' in ST_ESC
	EV_SS3_ENTRY       // 0x4F 'O' in ST_ESC
};

using OptEvent = std::optional <Events>;

using FSM = Basedlib::FSM::FSM <Basedlib::FSM::Enum<States>, Basedlib::FSM::Enum<Events>, Context
#ifdef BASEDVT_DEBUG
	, [] (std::string_view s) { std::print ("FSM Logger: {}\n", s); }
#endif
>;

enum class Actions {
	AC_PRINT,
	AC_EXECUTE,
	AC_CLEAR,
	AC_COLLECT,
	AC_PARAM,
	AC_ESC_DISPATCH,
	AC_CSI_DISPATCH,
	AC_SS3_DISPATCH
};

};
