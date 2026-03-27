#include "Decode.hpp"

#include <array>

#ifdef BASEDVT_DEBUG
#include <print>
#endif

namespace BasedVT {

std::optional<KeyInput> decode_print (const Token& t) {
	return KeyInput {
		.key = KeyInput::Key::CHAR,
		.ch = static_cast<char> (t.ch)
	};
}

std::optional<KeyInput> decode_exec (const Token& t) {
	return std::nullopt;
}

std::optional<KeyInput> decode_esc (const Token& t) {
	return std::nullopt;
}

std::optional<KeyInput> decode_csi (const Token& t) {
	return std::nullopt;
}

constexpr std::array decoder = {
	decode_print,
	decode_exec,
	decode_esc,
	decode_csi
};

std::optional<KeyInput> decode (const Token& t) {
#ifdef BASEDVT_DEBUG
	std::print ("Decode: {}\n", Token::PrettyType::to_string(t.type));
#endif
	return decoder[Token::PrettyType::idx(t.type)] (t);
}

}
