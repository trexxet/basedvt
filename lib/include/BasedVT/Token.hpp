#pragma once

#include <array>
#include <cstdint>

#include "Basedlib/PrettyEnum.hpp"

#ifdef BASEDVT_DEBUG
#include <format>
#include <string>
#endif

namespace BasedVT {

struct Token {
	enum class Type {
		PRINT,
		EXEC,
		ESC,
		CSI
	} type;
	using PrettyType = Basedlib::PrettyEnum <Type>;

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
		return std::format ("Type: {} ch: {} params: {} inter: {} private: {}",
			PrettyType::to_string (type),
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
			} (),
			privateMark > 0 ? std::format ("{}", privateMark) : "none"
		);
	}
#endif
};

}
