#include "BasedVT/Tokenizer.hpp"

#include "BasedVT/FSMDetail.hpp"
#include "FSMCallbacks.hpp"
#include "FSMClassifier.hpp"

#ifdef BASEDVT_ENABLE_TO_STRING
#include <format>
#endif

namespace BasedVT {

using namespace FSMDetail;

void Tokenizer::feed (uint8_t c) {
	if (OptEvent ev = classify_byte (c, ctx.mode, fsm.state())) {
		ctx.stage.currByte = c;
		fsm.event (ev.value());
	}
}

std::vector<Token> Tokenizer::feed_string (std::string_view str) {
	std::vector<Token> tokens;
	for (char c : str) {
		feed (c);
		if (OptToken token = get())
			tokens.emplace_back (std::move (*token));
	}
	return tokens;
}

Tokenizer::Tokenizer (Mode mode) : fsm (States::ST_GROUND, &ctx, make_callbacks ()) {
	switch_mode (mode);
}

#ifdef BASEDVT_ENABLE_TO_STRING
std::string Token::to_string() const noexcept {
	return std::format ("Type: {} ch: {} params: {} inter: {} private: {}",
		PrettyType::to_string (type),
		(int) ch,
		[this] () -> std::string {
			if (params.empty()) return "none";
			std::string str = {};
			for (int param : params)
				str.append (std::format("{} ", param));
			return str;
		} (),
		[this] () -> std::string {
			if (intermediates.empty()) return "none";
			std::string str = {};
			for (char intermediate : intermediates)
				str.append (std::format("{} ", intermediate));
			return str;
		} (),
		privateMark > 0 ? std::format ("{}", privateMark) : "none"
	);
}
#endif

}
