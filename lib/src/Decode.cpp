#include "Decode.hpp"

#include <array>

#ifdef BASEDVT_DEBUG
#include <print>
#endif

namespace BasedVT {

void apply_mods (KeyInput& k, uint8_t mods) {
	mods -= 1;
	if (mods >= 16) [[unlikely]] return;

	// TODO: add meta key (8)
	// k.meta = (mods & 0b1000) != 0;
	k.ctrl  = (mods & 0b0100) != 0;
	k.alt   = (mods & 0b0010) != 0;
	k.shift = (mods & 0b0001) != 0;
}

bool decode_tilde (KeyInput& k, uint8_t param) {
	switch (param) {
		case 2: k.key = KeyInput::Key::INSERT; break;
		case 3: k.key = KeyInput::Key::DELETE; break;
		case 5: k.key = KeyInput::Key::PAGE_UP; break;
		case 6: k.key = KeyInput::Key::PAGE_DOWN; break;
		case 15: k.key = KeyInput::Key::F5; break;
		case 17: k.key = KeyInput::Key::F6; break;
		case 18: k.key = KeyInput::Key::F7; break;
		case 19: k.key = KeyInput::Key::F8; break;
		case 20: k.key = KeyInput::Key::F9; break;
		case 21: k.key = KeyInput::Key::F10; break;
		case 23: k.key = KeyInput::Key::F11; break;
		case 24: k.key = KeyInput::Key::F12; break;
		default: return false;
	}
	return true;
}

std::optional<KeyInput> decode_print (const Token& t) {
	return KeyInput {
		.key = KeyInput::Key::CHAR,
		.ch = static_cast<char> (t.ch)
	};
}

std::optional<KeyInput> decode_exec (const Token& t) {
	switch (t.ch) {
		case 0x09: return KeyInput { .key = KeyInput::Key::TAB };
		case 0x0A:
		case 0x0D: return KeyInput { .key = KeyInput::Key::ENTER };
		case 0x1B: return KeyInput { .key = KeyInput::Key::ESCAPE };
		case 0x7F: return KeyInput { .key = KeyInput::Key::BACKSPACE };
		default:
			if (t.ch >= 0x01 && t.ch <= 0x1A)
				return KeyInput {
					.key = KeyInput::Key::CHAR,
					.ch = static_cast<char> ('a' + (t.ch - 1)),
					.ctrl = true
				};
	}
	return std::nullopt;
}

std::optional<KeyInput> decode_esc (const Token& t) {
	return KeyInput { .key = KeyInput::Key::ESCAPE };
}

std::optional<KeyInput> decode_csi (const Token& t) {
	KeyInput k;
	bool ok = true;

	if (t.paramsCount > 1)
		apply_mods (k, static_cast<uint8_t> (t.params[1]));

	switch (t.ch) {
		case 'A': k.key = KeyInput::Key::UP; break;
		case 'B': k.key = KeyInput::Key::DOWN; break;
		case 'C': k.key = KeyInput::Key::RIGHT; break;
		case 'D': k.key = KeyInput::Key::LEFT; break;
		case 'F': k.key = KeyInput::Key::END; break;
		case 'H': k.key = KeyInput::Key::HOME; break;
		case '~':
			if (t.paramsCount > 0)
				ok = decode_tilde (k, static_cast<uint8_t> (t.params[0]));
			break;
		default: ok = false; break;
	}

	if (ok) return k;
	return std::nullopt;
}

std::optional<KeyInput> decode_ss3 (const Token& t) {
	switch (t.ch) {
		case 'P': return KeyInput { .key = KeyInput::Key::F1 };
		case 'Q': return KeyInput { .key = KeyInput::Key::F2 };
		case 'R': return KeyInput { .key = KeyInput::Key::F3 };
		case 'S': return KeyInput { .key = KeyInput::Key::F4 };
	}
	return std::nullopt;
}

constexpr std::array decoder = {
	decode_print,
	decode_exec,
	decode_esc,
	decode_csi,
	decode_ss3
};

std::optional<KeyInput> decode (const Token& t) {
	std::optional<KeyInput> decoded = decoder[Token::PrettyType::idx(t.type)] (t);
#ifdef BASEDVT_DEBUG
	std::print ("Decode {}: {}\n",
		Token::PrettyType::to_string(t.type),
		decoded ? decoded->to_string() : "none"
	);
#endif
	return decoded;
}

}
