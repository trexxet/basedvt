#include "testTokenCommon.hpp"

Tokens tokenize (const std::string_view& str) {
	return tokenize_common <Mode::STRICT, false> (str);
}

Tokens tokenize_flush (const std::string_view& str) {
	return tokenize_common <Mode::STRICT, true> (str);
}

BT_SCENARIO_TEST (test_token_print) {
	BT_ASSERT_RC (Suite ("PRINT", cases <tokenize> (
		make_case ("a", "a", Token {Token::Type::PRINT, 'a'}),
		make_case ("[", "[", Token {Token::Type::PRINT, '['}),
		make_case ("O", "O", Token {Token::Type::PRINT, 'O'}),
		make_case ("/", "/", Token {Token::Type::PRINT, '/'}),
		make_case (";", ";", Token {Token::Type::PRINT, ';'}),
		make_case (":", ":", Token {Token::Type::PRINT, ':'}),
		make_case ("?", "?", Token {Token::Type::PRINT, '?'}),
		make_case ("~", "~", Token {Token::Type::PRINT, '~'}),
		make_case ("[1;2~", "[1;2~",
			Token {Token::Type::PRINT, '['},
			Token {Token::Type::PRINT, '1'},
			Token {Token::Type::PRINT, ';'},
			Token {Token::Type::PRINT, '2'},
			Token {Token::Type::PRINT, '~'}
		)
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_exec) {
	BT_ASSERT_RC (Suite ("EXEC", cases <tokenize> (
		make_case ("NUL", std::string_view("\0", 1), Token {Token::Type::EXEC, '\0'}),
		make_case ("CR",  "\r",   Token {Token::Type::EXEC, '\r'}),
		make_case ("LF",  "\n",   Token {Token::Type::EXEC, '\n'}),
		make_case ("TAB", "\t",   Token {Token::Type::EXEC, '\t'}),
		make_case ("CAN", "\x18", Token {Token::Type::EXEC, 0x18}),
		make_case ("SUB", "\x1A", Token {Token::Type::EXEC, 0x1A}),
		make_case ("DEL", "\x7F", Token {Token::Type::EXEC, 0x7F})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_esc) {
	BT_ASSERT_RC (Suite ("ESC", cases <tokenize> (
		make_case ("ESC",     "\e"),
		make_case ("ESC ESC", "\e\e"),
		make_case ("ESC A",   "\eA",  Token {Token::Type::ESC, 'A'}),
		make_case ("ESC 0",   "\e0",  Token {Token::Type::ESC, '0'}),
		make_case ("ESC ;",   "\e;",  Token {Token::Type::ESC, ';'}),
		make_case ("ESC :",   "\e:",  Token {Token::Type::ESC, ':'}),
		make_case ("ESC ?",   "\e?",  Token {Token::Type::ESC, '?'}),
		make_case ("ESC ~",   "\e~",  Token {Token::Type::ESC, '~'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("ESC flush", cases <tokenize_flush> (
		make_case ("ESC",     "\e",   Token {Token::Type::ESC}),
		make_case ("ESC ESC", "\e\e", Token {Token::Type::ESC}),
		make_case ("ESC A",   "\eA",  Token {Token::Type::ESC, 'A'})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_esc_inter) {
	BT_ASSERT_RC (Suite ("ESC Inter", cases <tokenize> (
		make_case ("ESC SP",   "\e "),
		make_case ("ESC /",    "\e/"),
		make_case ("ESC SP A", "\e A",  Token {Token::Type::ESC, 'A', {}, 0, {' '}}),
		make_case ("ESC !/ B", "\e!/B", Token {Token::Type::ESC, 'B', {}, 0, {'!', '/'}}),
		make_case ("ESC / 0",  "\e/0",  Token {Token::Type::ESC, '0', {}, 0, {'/'}}),
		make_case ("ESC / :",  "\e/:",  Token {Token::Type::ESC, ':', {}, 0, {'/'}}),
		make_case ("ESC / ?",  "\e/?",  Token {Token::Type::ESC, '?', {}, 0, {'/'}})
	)).run_rc());
	BT_ASSERT_RC (Suite ("ESC Inter flush (strict)", cases <tokenize_flush> (
		make_case ("ESC SP",   "\e "),
		make_case ("ESC /",    "\e/")
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_simple) {
	BT_ASSERT_RC (Suite ("CSI simple", cases <tokenize> (
		make_case ("CSI",         "\e["),
		make_case ("CSI CSI",     "\e[\e["),
		make_case ("CSI A",       "\e[A",       Token {Token::Type::CSI, 'A'}),
		make_case ("CSI DEL A",   "\e[\x7f""A", Token {Token::Type::CSI, 'A'}),
		make_case ("ESC DEL [ A", "\e\x7f[A",   Token {Token::Type::CSI, 'A'}),
		make_case ("ESC CSI A",   "\e\e[A",     Token {Token::Type::CSI, 'A'}),
		make_case ("CSI CSI A",   "\e[\e[A",    Token {Token::Type::CSI, 'A'}),
		make_case ("CSI A CSI B", "\e[A\e[B",   Token {Token::Type::CSI, 'A'}, Token {Token::Type::CSI, 'B'}),
		make_case ("CSI 1 CSI B", "\e[1\e[B",   Token {Token::Type::CSI, 'B'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("CSI simple flush", cases <tokenize_flush> (
		make_case ("CSI",         "\e["),
		make_case ("CSI A ESC",   "\e[A\e",     Token {Token::Type::CSI, 'A'}, Token {Token::Type::ESC}),
		make_case ("CSI A CSI",   "\e[A\e[",    Token {Token::Type::CSI, 'A'})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_param) {
	BT_ASSERT_RC (Suite ("CSI param", cases <tokenize> (
		make_case ("CSI 1;",   "\e[1;"),
		make_case ("CSI 3~",   "\e[3~",   Token {Token::Type::CSI, '~', {3}}),
		make_case ("CSI 24~",  "\e[24~",  Token {Token::Type::CSI, '~', {24}}),
		make_case ("CSI 1;5A", "\e[1;5A", Token {Token::Type::CSI, 'A', {1, 5}})
	)).run_rc());
	BT_ASSERT_RC (Suite ("CSI param flush", cases <tokenize_flush> (
		make_case ("CSI 1;",   "\e[1;")
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_param_wrong) {
	BT_ASSERT_RC (Suite ("CSI param wrong", cases <tokenize> (
		make_case ("CSI :A",  "\e[:A"),
		make_case ("CSI 1:A", "\e[1:A")
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_private) {
	BT_ASSERT_RC (Suite ("CSI private marker", cases <tokenize> (
		make_case ("CSI ?25h", "\e[?25h", Token {Token::Type::CSI, 'h', {25}, '?'}),
		make_case ("CSI ?25l", "\e[?25l", Token {Token::Type::CSI, 'l', {25}, '?'})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_inter) {
	BT_ASSERT_RC (Suite ("CSI inter", cases <tokenize> (
		make_case ("CSI ! 1",       "\e[!1"),
		make_case ("CSI SP ;",      "\e[ ;"),
		make_case ("CSI ! p",       "\e[!p",     Token {Token::Type::CSI, 'p', {}, 0, {'!'}}),
		make_case ("CSI 1 ! Q",     "\e[1!Q",    Token {Token::Type::CSI, 'Q', {1}, 0, {'!'}}),
		make_case ("CSI 12;5 SP R", "\e[12;5 R", Token {Token::Type::CSI, 'R', {12, 5}, 0, {' '}})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_ss3) {
	BT_ASSERT_RC (Suite ("SS3", cases <tokenize> (
		make_case ("SS3",       "\eO"),
		make_case ("SS3 A",     "\eOA",    Token {Token::Type::SS3, 'A'}),
		make_case ("SS3 B",     "\eOB",    Token {Token::Type::SS3, 'B'}),
		make_case ("SS3 R",     "\eOR",    Token {Token::Type::SS3, 'R'}),
		make_case ("SS3 S",     "\eOS",    Token {Token::Type::SS3, 'S'}),
		make_case ("SS3 SS3 P", "\eO\eOP", Token {Token::Type::SS3, 'P'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("SS3 flush", cases <tokenize_flush> (
		make_case ("SS3",       "\eO"),
		make_case ("SS3 P SS3", "\eOP\eO", Token {Token::Type::SS3, 'P'})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_inner_exec) {
	BT_ASSERT_RC (Suite ("Inner EXEC", cases <tokenize> (
		make_case ("CSI CAN A",    "\e[\x18""A", Token {Token::Type::EXEC, 0x18}, Token {Token::Type::PRINT, 'A'}),
		make_case ("ESC SP SUB",   "\e \x1A",    Token {Token::Type::EXEC, 0x1A}),
		make_case ("ESC SUB SP",   "\e\x1A ",    Token {Token::Type::EXEC, 0x1A}, Token {Token::Type::PRINT, ' '}),
		make_case ("ESC LF [1;5A", "\e\n[1;5A",  Token {Token::Type::EXEC, '\n'}, Token {Token::Type::CSI, 'A', {1, 5}}),
		make_case ("CSI LF 1;5A",  "\e[\n1;5A",  Token {Token::Type::EXEC, '\n'}, Token {Token::Type::CSI, 'A', {1, 5}}),
		make_case ("CSI 1 LF ;5A", "\e[1\n;5A",  Token {Token::Type::EXEC, '\n'}, Token {Token::Type::CSI, 'A', {1, 5}}),
		make_case ("CSI 1; LF 5A", "\e[1;\n5A",  Token {Token::Type::EXEC, '\n'}, Token {Token::Type::CSI, 'A', {1, 5}}),
		make_case ("CSI 1;5 LF A", "\e[1;5\nA",  Token {Token::Type::EXEC, '\n'}, Token {Token::Type::CSI, 'A', {1, 5}})
	)).run_rc());
	BT_ASSERT_RC (Suite ("Inner EXEC flush", cases <tokenize_flush> (
		make_case ("CSI CAN", "\e[\x18", Token {Token::Type::EXEC, 0x18}),
		make_case ("SS3 SUB", "\eO\x1A", Token {Token::Type::EXEC, 0x1A})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_mix) {
	BT_ASSERT_RC (Suite ("Mixed", cases <tokenize> (
		make_case ("a ESC",         "a\e",        Token {Token::Type::PRINT, 'a'}),
		make_case ("a CSI",         "a\e[",       Token {Token::Type::PRINT, 'a'}),
		make_case ("a CSI 1;5A CR", "a\e[1;5A\r", Token {Token::Type::PRINT, 'a'},       Token {Token::Type::CSI, 'A', {1, 5}}, Token {Token::Type::EXEC, '\r'}),
		make_case ("CSI 1;5A CR a", "\e[1;5A\ra", Token {Token::Type::CSI, 'A', {1, 5}}, Token {Token::Type::EXEC, '\r'},       Token {Token::Type::PRINT, 'a'}),
		make_case ("CR a CSI 1;5A", "\ra\e[1;5A", Token {Token::Type::EXEC, '\r'},       Token {Token::Type::PRINT, 'a'},       Token {Token::Type::CSI, 'A', {1, 5}}),
		make_case ("CR CSI 1;5A a", "\r\e[1;5Aa", Token {Token::Type::EXEC, '\r'},       Token {Token::Type::CSI, 'A', {1, 5}}, Token {Token::Type::PRINT, 'a'}),
		make_case ("a ESC / b",     "a\e/b",      Token {Token::Type::PRINT, 'a'},       Token {Token::Type::ESC, 'b', {}, 0, {'/'}}),
		make_case ("ESC SP CR",     "\e \r",      Token {Token::Type::EXEC, '\r'}),
		make_case ("CSI A ESC /",   "\e[A\e/",    Token {Token::Type::CSI, 'A'}),
		make_case ("SS3 P CSI 24~", "\eOP\e[24~", Token {Token::Type::SS3, 'P'},         Token {Token::Type::CSI, '~', {24}})
	)).run_rc());
	BT_ASSERT_RC (Suite ("Mixed flush", cases <tokenize_flush> (
		make_case ("a ESC",         "a\e",        Token {Token::Type::PRINT, 'a'}, Token {Token::Type::ESC}),
		make_case ("a CSI",         "a\e[",       Token {Token::Type::PRINT, 'a'})
	)).run_rc());
	BT_SUCCESS;
}

int main () {
	return Suite ("Token ASCII, mode STRICT", tests (
		BT_SUITE_SCENARIO (test_token_print),
		BT_SUITE_SCENARIO (test_token_exec),
		BT_SUITE_SCENARIO (test_token_esc),
		BT_SUITE_SCENARIO (test_token_esc_inter),
		BT_SUITE_SCENARIO (test_token_csi_simple),
		BT_SUITE_SCENARIO (test_token_csi_param),
		BT_SUITE_SCENARIO (test_token_csi_param_wrong),
		BT_SUITE_SCENARIO (test_token_csi_private),
		BT_SUITE_SCENARIO (test_token_csi_inter),
		BT_SUITE_SCENARIO (test_token_ss3),
		BT_SUITE_SCENARIO (test_token_inner_exec),
		BT_SUITE_SCENARIO (test_token_mix)
	)).run_rc();
}
