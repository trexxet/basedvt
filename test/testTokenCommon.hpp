#pragma once

#include "BasedVT/Tokenizer.hpp"
#include "Basedtest/Suite.hpp"

#include <format>
#include <string>
#include <string_view>

#include "Basedlib/Container/StaticVector.hpp"

// microslop moment
#ifdef __INTELLISENSE__
#pragma diag_suppress 304
#pragma diag_suppress 1574
#pragma diag_suppress 3133
#endif

using namespace Basedtest;
using namespace BasedVT;

using Tokens = Basedlib::StaticVector<Token, 8>;
using Mode = Tokenizer::Mode;

#ifdef BASEDVT_ENABLE_TO_STRING
namespace Basedlib {
std::string to_string (const Tokens& tokens);
}
#endif

#ifndef BASEDVT_TOKEN_TEST_COMMON
extern
#endif
Tokenizer tokenizer;

template <Mode mode, bool flush = false>
Tokens tokenize_common (std::string_view str) {
	tokenizer.switch_mode (mode);
	auto tokens = Tokens (tokenizer.feed_string (str));
	if constexpr (flush) {
		if (OptToken flushToken = tokenizer.flush())
			tokens.emplace_back (std::move (*flushToken));
	}
	return tokens;
}

template <typename... Args>
constexpr auto make_case (std::string_view name, std::string_view input, Args... args) {
	return ValueCase {name, input, Tokens {std::forward<Args>(args)...}};
}
