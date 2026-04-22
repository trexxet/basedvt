#include "BasedVT/Decode.hpp"
#include "Basedtest/Suite.hpp"

#include <optional>

using namespace Basedtest;
using namespace BasedVT;
using namespace BasedVT::InputEvent;

namespace BasedVT {
#ifdef BASEDVT_ENABLE_TO_STRING
namespace InputEvent {
std::string to_string (const OptKeyInput& optkey) {
	if (!optkey) return "none";
	return optkey->to_string();
}
}
#endif
bool apply_mods (KeyInput& k, uint8_t mods);
}

constexpr auto make_case (std::string_view name, Token input, KeyInput key) {
	return ValueCase {name, input, OptKeyInput (key)};
}

constexpr auto make_case (std::string_view name, Token input) {
	return ValueCase {name, input, OptKeyInput (std::nullopt)};
}

BT_SCENARIO_TEST (test_decode_char) {
	BT_ASSERT_RC (Suite ("CHAR", cases <decode_key> (
		// ASCII
		make_case ("a",  Token {Token::Type::PRINT, 'a'}, KeyInput {KeyInput::Key::CHAR, 'a'}),
		make_case ("A",  Token {Token::Type::PRINT, 'A'}, KeyInput {KeyInput::Key::CHAR, 'A'}),
		make_case ("z",  Token {Token::Type::PRINT, 'z'}, KeyInput {KeyInput::Key::CHAR, 'z'}),
		make_case ("0",  Token {Token::Type::PRINT, '0'}, KeyInput {KeyInput::Key::CHAR, '0'}),
		make_case ("[",  Token {Token::Type::PRINT, '['}, KeyInput {KeyInput::Key::CHAR, '['}),
		make_case ("~",  Token {Token::Type::PRINT, '~'}, KeyInput {KeyInput::Key::CHAR, '~'}),
		make_case ("SP", Token {Token::Type::PRINT, ' '}, KeyInput {KeyInput::Key::CHAR, ' '}),
		// High
		make_case ("0x80", Token {Token::Type::PRINT, 0x80}, KeyInput {KeyInput::Key::CHAR, 0x80}),
		make_case ("0x8F", Token {Token::Type::PRINT, 0x8F}, KeyInput {KeyInput::Key::CHAR, 0x8F}),
		make_case ("0x9B", Token {Token::Type::PRINT, 0x9B}, KeyInput {KeyInput::Key::CHAR, 0x9B}),
		make_case ("0xA0", Token {Token::Type::PRINT, 0xA0}, KeyInput {KeyInput::Key::CHAR, 0xA0}),
		make_case ("0xFF", Token {Token::Type::PRINT, 0xFF}, KeyInput {KeyInput::Key::CHAR, 0xFF}),
		// DEL - ignore
		make_case ("DEL",  Token {Token::Type::PRINT, 0x7F}),
		// Non-plain - ignore
		make_case ("a param",   Token {Token::Type::PRINT, 'a', {1}}),
		make_case ("a private", Token {Token::Type::PRINT, 'a', {}, '?'}),
		make_case ("a inter",   Token {Token::Type::PRINT, 'a', {}, 0, {' '}})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_decode_exec) {
	BT_ASSERT_RC (Suite ("EXEC", cases <decode_key> (
		// ASCII
		make_case ("TAB",        Token {Token::Type::EXEC, '\t'},   KeyInput {KeyInput::Key::TAB}),
		make_case ("CR",         Token {Token::Type::EXEC, '\r'},   KeyInput {KeyInput::Key::ENTER}),
		make_case ("LF",         Token {Token::Type::EXEC, '\n'},   KeyInput {KeyInput::Key::ENTER}),
		make_case ("ESCAPE",     Token {Token::Type::EXEC, '\e'},   KeyInput {KeyInput::Key::ESCAPE}),
		make_case ("BACKSPACE",  Token {Token::Type::EXEC, '\x7f'}, KeyInput {KeyInput::Key::BACKSPACE}),
		// ASCII ctrl keys
		make_case ("NUL",        Token {Token::Type::EXEC, '\0'},   KeyInput {.key = KeyInput::Key::CHAR, .byte = ' ',  .ctrl = true}),
		make_case ("\\ ctrl",    Token {Token::Type::EXEC, 0x1C},   KeyInput {.key = KeyInput::Key::CHAR, .byte = '\\', .ctrl = true}),
		make_case ("] ctrl",     Token {Token::Type::EXEC, 0x1D},   KeyInput {.key = KeyInput::Key::CHAR, .byte = ']',  .ctrl = true}),
		make_case ("^ ctrl",     Token {Token::Type::EXEC, 0x1E},   KeyInput {.key = KeyInput::Key::CHAR, .byte = '^',  .ctrl = true}),
		make_case ("_ ctrl",     Token {Token::Type::EXEC, 0x1F},   KeyInput {.key = KeyInput::Key::CHAR, .byte = '_',  .ctrl = true}),
		// High - ignore
		make_case ("0x80",       Token {Token::Type::EXEC, 0x80}),
		make_case ("0x9B",       Token {Token::Type::EXEC, 0x9B}),
		// Non-plain - ignore
		make_case ("CR param",   Token {Token::Type::EXEC, '\r', {1}}),
		make_case ("CR private", Token {Token::Type::EXEC, '\r', {}, '?'}),
		make_case ("CR inter",   Token {Token::Type::EXEC, '\r', {}, 0, {' '}})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_decode_esc) {
	BT_ASSERT_RC (Suite ("ESC", cases <decode_key> (
		// ESC & alt mod
		make_case ("ESCAPE",   Token {Token::Type::ESC},        KeyInput {KeyInput::Key::ESCAPE}),
		make_case ("alt A",    Token {Token::Type::ESC, 'A'},   KeyInput {.key = KeyInput::Key::CHAR, .byte = 'A',  .alt = true}),
		make_case ("alt a",    Token {Token::Type::ESC, 'a'},   KeyInput {.key = KeyInput::Key::CHAR, .byte = 'a',  .alt = true}),
		make_case ("alt 1",    Token {Token::Type::ESC, '1'},   KeyInput {.key = KeyInput::Key::CHAR, .byte = '1',  .alt = true}),
		make_case ("alt [",    Token {Token::Type::ESC, '['},   KeyInput {.key = KeyInput::Key::CHAR, .byte = '[',  .alt = true}),
		make_case ("alt O",    Token {Token::Type::ESC, 'O'},   KeyInput {.key = KeyInput::Key::CHAR, .byte = 'O',  .alt = true}),
		make_case ("alt /",    Token {Token::Type::ESC, '/'},   KeyInput {.key = KeyInput::Key::CHAR, .byte = '/',  .alt = true}),
		make_case ("alt SP",   Token {Token::Type::ESC, ' '},   KeyInput {.key = KeyInput::Key::CHAR, .byte = ' ',  .alt = true}),
		make_case ("alt 0x8F", Token {Token::Type::ESC, 0x8F},  KeyInput {.key = KeyInput::Key::CHAR, .byte = 0x8F, .alt = true}),
		make_case ("alt 0x9B", Token {Token::Type::ESC, 0x9B},  KeyInput {.key = KeyInput::Key::CHAR, .byte = 0x9B, .alt = true}),
		// DEL, C0 - ignore
		make_case ("DEL",      Token {Token::Type::ESC, 0x7F}),
		make_case ("0x1F",     Token {Token::Type::ESC, 0x1F}),
		// Non-plain - ignore
		make_case ("alt A param",   Token {Token::Type::ESC, 'A', {1}}),
		make_case ("alt A private", Token {Token::Type::ESC, 'A', {}, '?'}),
		make_case ("alt A inter",   Token {Token::Type::ESC, 'A', {}, 0, {' '}})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_decode_csi_simple) {
	BT_ASSERT_RC (Suite ("CSI simple", cases <decode_key> (
		make_case ("UP",    Token {Token::Type::CSI, 'A'}, KeyInput {KeyInput::Key::UP}),
		make_case ("DOWN",  Token {Token::Type::CSI, 'B'}, KeyInput {KeyInput::Key::DOWN}),
		make_case ("RIGHT", Token {Token::Type::CSI, 'C'}, KeyInput {KeyInput::Key::RIGHT}),
		make_case ("LEFT",  Token {Token::Type::CSI, 'D'}, KeyInput {KeyInput::Key::LEFT}),
		make_case ("BEGIN", Token {Token::Type::CSI, 'E'}, KeyInput {KeyInput::Key::BEGIN}),
		make_case ("END",   Token {Token::Type::CSI, 'F'}, KeyInput {KeyInput::Key::END}),
		make_case ("HOME",  Token {Token::Type::CSI, 'H'}, KeyInput {KeyInput::Key::HOME}),
		make_case ("F1",    Token {Token::Type::CSI, 'P'}, KeyInput {KeyInput::Key::F1}),
		make_case ("F2",    Token {Token::Type::CSI, 'Q'}, KeyInput {KeyInput::Key::F2}),
		make_case ("F3",    Token {Token::Type::CSI, 'R'}, KeyInput {KeyInput::Key::F3}),
		make_case ("F4",    Token {Token::Type::CSI, 'S'}, KeyInput {KeyInput::Key::F4})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_decode_csi_tilde) {
	BT_ASSERT_RC (Suite ("CSI tilde", cases <decode_key> (
		make_case ("HOME",      Token {Token::Type::CSI, '~', {1}},  KeyInput {KeyInput::Key::HOME}),
		make_case ("INSERT",    Token {Token::Type::CSI, '~', {2}},  KeyInput {KeyInput::Key::INSERT}),
		make_case ("DELETE",    Token {Token::Type::CSI, '~', {3}},  KeyInput {KeyInput::Key::DELETE}),
		make_case ("END",       Token {Token::Type::CSI, '~', {4}},  KeyInput {KeyInput::Key::END}),
		make_case ("PAGE_UP",   Token {Token::Type::CSI, '~', {5}},  KeyInput {KeyInput::Key::PAGE_UP}),
		make_case ("PAGE_DOWN", Token {Token::Type::CSI, '~', {6}},  KeyInput {KeyInput::Key::PAGE_DOWN}),
		make_case ("F1",        Token {Token::Type::CSI, '~', {11}}, KeyInput {KeyInput::Key::F1}),
		make_case ("F2",        Token {Token::Type::CSI, '~', {12}}, KeyInput {KeyInput::Key::F2}),
		make_case ("F3",        Token {Token::Type::CSI, '~', {13}}, KeyInput {KeyInput::Key::F3}),
		make_case ("F4",        Token {Token::Type::CSI, '~', {14}}, KeyInput {KeyInput::Key::F4}),
		make_case ("F5",        Token {Token::Type::CSI, '~', {15}}, KeyInput {KeyInput::Key::F5}),
		make_case ("F6",        Token {Token::Type::CSI, '~', {17}}, KeyInput {KeyInput::Key::F6}),
		make_case ("F7",        Token {Token::Type::CSI, '~', {18}}, KeyInput {KeyInput::Key::F7}),
		make_case ("F8",        Token {Token::Type::CSI, '~', {19}}, KeyInput {KeyInput::Key::F8}),
		make_case ("F9",        Token {Token::Type::CSI, '~', {20}}, KeyInput {KeyInput::Key::F9}),
		make_case ("F10",       Token {Token::Type::CSI, '~', {21}}, KeyInput {KeyInput::Key::F10}),
		make_case ("F11",       Token {Token::Type::CSI, '~', {23}}, KeyInput {KeyInput::Key::F11}),
		make_case ("F12",       Token {Token::Type::CSI, '~', {24}}, KeyInput {KeyInput::Key::F12})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_decode_ss3) {
	BT_ASSERT_RC (Suite ("SS3", cases <decode_key> (
		make_case ("F1", Token {Token::Type::SS3, 'P'}, KeyInput {KeyInput::Key::F1}),
		make_case ("F2", Token {Token::Type::SS3, 'Q'}, KeyInput {KeyInput::Key::F2}),
		make_case ("F3", Token {Token::Type::SS3, 'R'}, KeyInput {KeyInput::Key::F3}),
		make_case ("F4", Token {Token::Type::SS3, 'S'}, KeyInput {KeyInput::Key::F4}),
		make_case ("Z",  Token {Token::Type::SS3, 'Z'}),
		// Non-plain - ignore
		make_case ("F1 param",   Token {Token::Type::SS3, 'P', {1}}),
		make_case ("F1 private", Token {Token::Type::SS3, 'P', {}, '?'}),
		make_case ("F1 inter",   Token {Token::Type::SS3, 'P', {}, 0, {' '}})
	)).run_rc());
	BT_SUCCESS;
}

struct Mods {
	bool ok = true;
	uint8_t code;
	bool ctrl = false, alt = false, shift = false;
};

BT_ASSERT_TEST (test_decode_mods_single, Mods, mods) {
	KeyInput k = {.key = KeyInput::Key::UP, .byte = 'A'};

	BT_ASSERT_EQ ((apply_mods (k, mods.code)), mods.ok);
	BT_ASSERT_EQ (k.ctrl, mods.ctrl);
	BT_ASSERT_EQ (k.alt, mods.alt);
	BT_ASSERT_EQ (k.shift, mods.shift);

	BT_ASSERT_EQ (k.key, KeyInput::Key::UP);
	BT_ASSERT_EQ (k.byte, 'A');

	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_decode_mods) {
	BT_ASSERT_RC (Suite ("Mods", cases <test_decode_mods_single> (
		AssertCase {"none0",          Mods {.ok = false, .code = 0}},
		AssertCase {"none1",          Mods {.code = 1}},
		AssertCase {"shift",          Mods {.code = 2, .shift = true}},
		AssertCase {"alt",            Mods {.code = 3, .alt = true}},
		AssertCase {"alt+shift",      Mods {.code = 4, .alt = true, .shift = true}},
		AssertCase {"ctrl",           Mods {.code = 5, .ctrl = true}},
		AssertCase {"ctrl+shift",     Mods {.code = 6, .ctrl = true, .shift = true}},
		AssertCase {"ctrl+alt",       Mods {.code = 7, .ctrl = true, .alt = true}},
		AssertCase {"ctrl+alt+shift", Mods {.code = 8, .ctrl = true, .alt = true, .shift = true}},
		// TODO: update when Meta modifier supported
		AssertCase {"none9",          Mods {.code = 9}},
		AssertCase {"none99",         Mods {.ok = false, .code = 99}}
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_decode_csi_mods) {
	BT_ASSERT_RC (Suite ("CSI mods", cases <decode_key> (
		make_case ("shift UP",             Token {Token::Type::CSI, 'A', {1, 2}},  KeyInput {.key = KeyInput::Key::UP,        .shift = true}),
		make_case ("alt UP",               Token {Token::Type::CSI, 'A', {1, 3}},  KeyInput {.key = KeyInput::Key::UP,        .alt = true}),
		make_case ("ctrl DOWN",            Token {Token::Type::CSI, 'B', {1, 5}},  KeyInput {.key = KeyInput::Key::DOWN,      .ctrl = true}),
		make_case ("alt+shift DOWN",       Token {Token::Type::CSI, 'B', {1, 4}},  KeyInput {.key = KeyInput::Key::DOWN,      .alt = true, .shift = true}),
		make_case ("ctrl+shift RIGHT",     Token {Token::Type::CSI, 'C', {1, 6}},  KeyInput {.key = KeyInput::Key::RIGHT,     .ctrl = true, .shift = true}),
		make_case ("ctrl+alt+shift LEFT",  Token {Token::Type::CSI, 'D', {1, 8}},  KeyInput {.key = KeyInput::Key::LEFT,      .ctrl = true, .alt = true, .shift = true}),
		make_case ("ctrl DELETE",          Token {Token::Type::CSI, '~', {3, 5}},  KeyInput {.key = KeyInput::Key::DELETE,    .ctrl = true}),
		make_case ("shift F1",             Token {Token::Type::CSI, '~', {11, 2}}, KeyInput {.key = KeyInput::Key::F1,        .shift = true}),
		make_case ("alt+shift PAGE_UP",    Token {Token::Type::CSI, '~', {5, 4}},  KeyInput {.key = KeyInput::Key::PAGE_UP,   .alt = true, .shift = true}),
		make_case ("ctrl+shift PAGE_DOWN", Token {Token::Type::CSI, '~', {6, 6}},  KeyInput {.key = KeyInput::Key::PAGE_DOWN, .ctrl = true, .shift = true}),
		make_case ("ctrl+alt+shift F12",   Token {Token::Type::CSI, '~', {24, 8}}, KeyInput {.key = KeyInput::Key::F12,       .ctrl = true, .alt = true, .shift = true})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_decode_csi_wrong) {
	BT_ASSERT_RC (Suite ("CSI wrong", cases <decode_key> (
		make_case ("CSI 2 B",      Token {Token::Type::CSI, 'B', {2}}),
		make_case ("CSI 2;5 B",    Token {Token::Type::CSI, 'B', {2, 5}}),
		make_case ("CSI 1;99 B",   Token {Token::Type::CSI, 'B', {1, 99}}),
		make_case ("CSI ? B",      Token {Token::Type::CSI, 'B', {}, '?'}),
		make_case ("CSI ! B",      Token {Token::Type::CSI, 'B', {}, 0, {'!'}}),
		make_case ("CSI ~",        Token {Token::Type::CSI, '~'}),
		make_case ("CSI 99 ~",     Token {Token::Type::CSI, '~', {99}}),
		make_case ("CSI 24;99 ~",  Token {Token::Type::CSI, '~', {24, 99}}),
		make_case ("CSI 24;5;7 ~", Token {Token::Type::CSI, '~', {24, 5, 7}}),
		make_case ("CSI ? ~",      Token {Token::Type::CSI, '~', {}, '?'}),
		make_case ("CSI ! ~",      Token {Token::Type::CSI, '~', {}, 0, {'!'}})
	)).run_rc());
	BT_SUCCESS;
}

int main () {
	return Suite ("Decode", tests (
		BT_SUITE_SCENARIO (test_decode_char),
		BT_SUITE_SCENARIO (test_decode_exec),
		BT_SUITE_SCENARIO (test_decode_esc),
		BT_SUITE_SCENARIO (test_decode_csi_simple),
		BT_SUITE_SCENARIO (test_decode_csi_tilde),
		BT_SUITE_SCENARIO (test_decode_ss3),
		BT_SUITE_SCENARIO (test_decode_mods),
		BT_SUITE_SCENARIO (test_decode_csi_mods),
		BT_SUITE_SCENARIO (test_decode_csi_wrong)
	)).run_rc();
}
