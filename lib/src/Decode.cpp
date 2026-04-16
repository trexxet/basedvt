#include "BasedVT/Decode.hpp"

#include <array>

#ifdef BASEDVT_DEBUG
#include <print>
#endif

namespace BasedVT {

bool apply_mods (KeyInput& k, uint8_t mods) {
	mods -= 1;
	if (mods >= 16) [[unlikely]] return false;

	// TODO: add meta key (8)
	// k.meta = (mods & 0b1000) != 0;
	k.ctrl  = (mods & 0b0100) != 0;
	k.alt   = (mods & 0b0010) != 0;
	k.shift = (mods & 0b0001) != 0;
	return true;
}

OptKeyInput decode_print (const Token& t) {
	return KeyInput {
		.key = KeyInput::Key::CHAR,
		.ch = static_cast<char> (t.ch)
	};
}

OptKeyInput decode_exec (const Token& t) {
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

OptKeyInput decode_esc (const Token& t) {
	return KeyInput { .key = KeyInput::Key::ESCAPE };
}

bool decode_csi_tilde (KeyInput& k, const Token& t) {
	if (t.privateMark) return false;
	if (!t.intermediates.empty()) return false;
	if (t.params.empty() || t.params.size() > 2) return false;
	if (t.params.size() == 2)
		if (!apply_mods (k, static_cast<uint8_t> (t.params[1])))
			return false;

	switch (t.params[0]) {
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

bool decode_csi_arrow (KeyInput& k, const Token& t, KeyInput::Key key) {
	if (t.privateMark) return false;
	if (!t.intermediates.empty()) return false;
	if (!t.params.empty()) {
		if (t.params.size() == 2 && t.params[0] == 1) {
			if (!apply_mods (k, static_cast<uint8_t> (t.params[1])))
				return false;
		}
		else return false;
	}
	k.key = key;
	return true;
}

OptKeyInput decode_csi (const Token& t) {
	KeyInput k;
	bool ok = true;

	switch (t.ch) {
		case 'A': ok = decode_csi_arrow (k, t, KeyInput::Key::UP);    break;
		case 'B': ok = decode_csi_arrow (k, t, KeyInput::Key::DOWN);  break;
		case 'C': ok = decode_csi_arrow (k, t, KeyInput::Key::RIGHT); break;
		case 'D': ok = decode_csi_arrow (k, t, KeyInput::Key::LEFT);  break;
		case 'F': ok = decode_csi_arrow (k, t, KeyInput::Key::END);   break;
		case 'H': ok = decode_csi_arrow (k, t, KeyInput::Key::HOME);  break;
		case '~': ok = decode_csi_tilde (k, t); break;
		default:  ok = false; break;
	}

	if (ok) return k;
	return std::nullopt;
}

OptKeyInput decode_ss3 (const Token& t) {
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

OptKeyInput decode (const Token& t) {
	OptKeyInput decoded = decoder[Token::PrettyType::idx(t.type)] (t);
#ifdef BASEDVT_DEBUG
	std::print ("Decode {}: {}\n",
		Token::PrettyType::to_string(t.type),
		decoded ? decoded->to_string() : "none"
	);
#endif
	return decoded;
}

}
