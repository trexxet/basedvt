#pragma once

#include "Basedlib/FSM.hpp"

#include <array>
#include <cstdint>
#include <string>

#ifdef BASEDVT_DEBUG
#include <print>
#include <string_view>
#endif

namespace BasedVT::FSMDetail {

struct Token {
	enum class Type {
		NONE,
		PRINT,
		EXEC,
		ESC,
		CSI
	} type = Type::NONE;

	uint8_t ch = 0;
	uint8_t privateMark = 0;
	std::array<int, 16> params = {};
	uint8_t paramsCount = 0;
	std::array<char, 4> intermediates = {};
	uint8_t intermediatesCount = 0;

	void add_param (int* param) noexcept {
		if (paramsCount < params.size())
			params[paramsCount++] = (*param < 0) ? 0 : *param;
		*param = -1;
	}

	void add_intermediate (char intermediate) {
		if (intermediatesCount < intermediates.size())
			intermediates[intermediatesCount++] = intermediate;
	}

#ifdef BASEDVT_DEBUG
	const std::string to_string() const noexcept {
		return std::format ("Type: {} ch: {} params: {} inter: {}",
			Basedlib::PrettyEnum<Type>::to_string (type),
			(int) ch,
			[this] () -> std::string {
				if (paramsCount == 0) return "none";
				std::string str = {};
				for (uint8_t i = 0; i < paramsCount; i++)
					str.append (std::format("{} ", params[i]));
				return str;
			} (),
			[this] () -> std::string {
				if (intermediatesCount == 0) return "none";
				std::string str = {};
				for (uint8_t i = 0; i < intermediatesCount; i++)
					str.append (std::format("{} ", intermediates[i]));
				return str;
			} ()
		);
	}
#endif
};

struct Context {
	uint8_t currByte = 0;
	int currParam = -1;
	Token token = {};
	bool ready = false;
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
	EV_CSI_ENTRY,    // 0x5B in ST_ESC
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
