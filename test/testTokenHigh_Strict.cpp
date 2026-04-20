#include "testTokenCommon.hpp"

Tokens tokenize (const std::string_view& str) {
	return tokenize_common <Mode::STRICT, false> (str);
}

Tokens tokenize_flush (const std::string_view& str) {
	return tokenize_common <Mode::STRICT, true> (str);
}

BT_SCENARIO_TEST (test_token_c1_exec) {
	BT_ASSERT_RC (Suite ("C1 EXEC", cases <tokenize> (
		make_case ("CSI 1 PAD A",  "\e[1\x80""A", Token {Token::Type::EXEC, 0x80}, Token {Token::Type::PRINT, 'A'}),
		make_case ("ESC SP PAD A", "\e \x80""A",  Token {Token::Type::EXEC, 0x80}, Token {Token::Type::PRINT, 'A'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("C1 EXEC flush", cases <tokenize_flush> (
		make_case ("ESC SP SCI",  "\e \x9A",     Token {Token::Type::EXEC, 0x9A})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_c1_csi) {
	BT_ASSERT_RC (Suite ("C1 CSI", cases <tokenize> (
		make_case ("CSI",         "\x9B"),
		make_case ("CSI A",       "\x9B""A",     Token {Token::Type::CSI, 'A'}),
		make_case ("CSI 1 CSI A", "\e[1\x9B""A", Token {Token::Type::CSI, 'A'}),
		make_case ("A CSI",       "A\x9B",       Token {Token::Type::PRINT, 'A'}),
		make_case ("A CSI A",     "A\x9B""A",    Token {Token::Type::PRINT, 'A'}, Token {Token::Type::CSI, 'A'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("C1 CSI flush", cases <tokenize_flush> (
		make_case ("CSI",    "\x9B"),
		make_case ("CSI 1;", "\x9B""1;")
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_c1_ss3) {
	BT_ASSERT_RC (Suite ("C1 SS3", cases <tokenize> (
		make_case ("SS3",          "\x8F"),
		make_case ("SS3 P",        "\x8F""P",    Token {Token::Type::SS3, 'P'}),
		make_case ("ESC SP SS3 P", "\e \x8F""P", Token {Token::Type::SS3, 'P'}),
		make_case ("A SS3",        "A\x8F",      Token {Token::Type::PRINT, 'A'}),
		make_case ("A SS3 P",      "A\x8F""P",   Token {Token::Type::PRINT, 'A'}, Token {Token::Type::SS3, 'P'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("C1 SS3 flush", cases <tokenize_flush> (
		make_case ("SS3", "\x8F")
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_c1_unsupported) {
	BT_ASSERT_RC (Suite ("C1 unsupported", cases <tokenize> (
		make_case ("0x90", "\x90"),
		make_case ("0x98", "\x98"),
		make_case ("0x9C", "\x9C"),
		make_case ("0x9D", "\x9D"),
		make_case ("0x9E", "\x9E"),
		make_case ("0x9F", "\x9F")
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

int main () {
	return Suite ("Token High, mode STRICT", tests (
		BT_SUITE_SCENARIO (test_token_c1_exec),
		BT_SUITE_SCENARIO (test_token_c1_csi),
		BT_SUITE_SCENARIO (test_token_c1_ss3),
		BT_SUITE_SCENARIO (test_token_c1_unsupported),
		BT_SUITE_SCENARIO (test_token_gr_print)
	)).run_rc();
}
