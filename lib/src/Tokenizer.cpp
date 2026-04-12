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

std::vector<Token> Tokenizer::feed_string (std::string_view str) {
	std::vector<Token> tokens;
	for (char c : str) {
		feed (c);
		if (std::optional<Token> token = get())
			tokens.emplace_back (std::move (*token));
	}
	return tokens;
}

void Tokenizer::reset () noexcept {
	ctx = {};
	fsm.switch_state (States::ST_GROUND);
}

Tokenizer::Tokenizer () : fsm (States::ST_GROUND, &ctx, make_callbacks ()) { }

#ifdef BASEDVT_DEBUG
const std::string Token::to_string() const noexcept {
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

}
