#include "testTokenCommon.hpp"

Tokens tokenize (const std::string_view& str) {
	return tokenize_common <Mode::INPUT, false> (str);
}

Tokens tokenize_flush (const std::string_view& str) {
	return tokenize_common <Mode::INPUT, true> (str);
}

BT_SCENARIO_TEST (test_token_c1_print) {
	BT_ASSERT_RC (Suite ("C1 PRINT", cases <tokenize> (
		make_case ("0x80", "\x80", Token {Token::Type::PRINT, 0x80}),
		make_case ("0x8F", "\x8F", Token {Token::Type::PRINT, 0x8F}),
		make_case ("0x9B", "\x9B", Token {Token::Type::PRINT, 0x9B}),
		make_case ("0x9F", "\x9F", Token {Token::Type::PRINT, 0x9F})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_gr_print) {
	BT_ASSERT_RC (Suite ("GR PRINT", cases <tokenize> (
		make_case ("0xA0", "\xA0", Token {Token::Type::PRINT, 0xA0}),
		make_case ("0xB4", "\xB4", Token {Token::Type::PRINT, 0xB4}),
		make_case ("0xC8", "\xC8", Token {Token::Type::PRINT, 0xC8}),
		make_case ("0xDA", "\xDA", Token {Token::Type::PRINT, 0xDA}),
		make_case ("0xEC", "\xEC", Token {Token::Type::PRINT, 0xEC}),
		make_case ("0xFF", "\xFF", Token {Token::Type::PRINT, 0xFF})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_mix_gl_gr_print) {
	BT_ASSERT_RC (Suite ("Mix GL/GR PRINT", cases <tokenize> (
		make_case ("a 0xA0 b",    "a\xA0""b",    Token {Token::Type::PRINT, 'a'},  Token {Token::Type::PRINT, 0xA0}, Token {Token::Type::PRINT, 'b'}),
		make_case ("0xB4 z 0xC8", "\xB4""z\xC8", Token {Token::Type::PRINT, 0xB4}, Token {Token::Type::PRINT, 'z'},  Token {Token::Type::PRINT, 0xC8})
	)).run_rc());
	BT_SUCCESS;
}

int main () {
	return Suite ("Token High, mode INPUT", tests (
		BT_SUITE_SCENARIO (test_token_c1_print),
		BT_SUITE_SCENARIO (test_token_gr_print),
		BT_SUITE_SCENARIO (test_token_mix_gl_gr_print)
	)).run_rc();
}
