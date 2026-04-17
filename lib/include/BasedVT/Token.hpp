#pragma once

#include <cstdint>
#include <optional>

#include "Basedlib/Class.hpp"
#include "Basedlib/Container/StaticVector.hpp"
#include "Basedlib/Meta/PrettyEnum.hpp"

namespace BasedVT {

struct Token {
	enum class Type {
		PRINT,
		EXEC,
		ESC,
		CSI,
		SS3
	} type;
	using PrettyType = Basedlib::PrettyEnum <Type>;

	uint8_t ch = 0;
	Basedlib::StaticVector<int, 16> params = {};
	uint8_t privateMark = 0;
	Basedlib::StaticVector<char, 4> intermediates = {};

	void add_param (int* param) noexcept {
		if (!params.full())
			params.emplace_back ((*param < 0) ? 0 : *param);
		*param = -1;
	}

	void add_intermediate (char intermediate) {
		if (!intermediates.full())
			intermediates.emplace_back (intermediate);
	}

#ifdef BASEDVT_DEBUG
	std::string to_string() const noexcept;
#endif

	BASED_CLASS_DEFAULT_EQUALITY (Token);
};

using OptToken = std::optional<Token>;

}
