#include "BasedVT/Tokenizer.hpp"

#include "BasedVT/FSMDetail.hpp"
#include "FSMCallbacks.hpp"

#ifdef BASEDVT_DEBUG
#include <format>
#include <string>
#endif

namespace BasedVT {

using namespace FSMDetail;

void Tokenizer::feed (uint8_t c) {
	if (std::optional<Events> ev = byte_to_event (c, fsm.state())) {
		ctx.currByte = c;
		fsm.event (ev.value());
	}
}

OptToken Tokenizer::flush () noexcept {
	return std::nullopt;
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

void Tokenizer::reset () noexcept {
	ctx.reset();
	fsm.switch_state (States::ST_GROUND);
}

Tokenizer::Tokenizer (Mode mode) : fsm (States::ST_GROUND, &ctx, make_callbacks ()) {
	ctx.reset();
	ctx.mode = mode;
}

#ifdef BASEDVT_DEBUG
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
