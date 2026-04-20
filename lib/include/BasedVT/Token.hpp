#pragma once

#include <cstdint>
#include <limits>
#include <optional>
#include <utility>

#include "Basedlib/Class.hpp"
#include "Basedlib/Container/StaticVector.hpp"
#include "Basedlib/Meta/PrettyEnum.hpp"

#ifdef BASEDVT_ENABLE_TO_STRING
#include <string>
#endif

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

	uint8_t byte = 0;
	Basedlib::StaticVector<int, 16> params = {};
	uint8_t privateMark = 0;
	Basedlib::StaticVector<char, 4> intermediates = {};

#ifdef BASEDVT_ENABLE_TO_STRING
	std::string to_string() const noexcept;
#endif

	BASED_CLASS_DEFAULT_EQUALITY (Token);
};

using OptToken = std::optional<Token>;

struct TokenStage {
	uint8_t currByte = 0;
	int currParam = -1;
	Token token = {};

	void append_param () noexcept {
		if (currParam < 0) currParam = 0;
		if (currParam < std::numeric_limits<int16_t>::max())
			currParam = currParam * 10 + (currByte - '0');
	}

	void commit_param () noexcept {
		if (!token.params.full())
			token.params.emplace_back ((currParam < 0) ? 0 : currParam);
		currParam = -1;
	}

	void commit_intermediate () noexcept {
		if (!token.intermediates.full())
			token.intermediates.emplace_back (static_cast<char> (currByte));
	}

	void commit_private () noexcept { token.privateMark = currByte; }
	void commit_ch () noexcept { token.byte = currByte; }
	void set_type (Token::Type type) noexcept { token.type = type; }

	Token ready () noexcept {
		return std::exchange (token, {});
	}
};

}
