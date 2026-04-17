#include "BasedVT/Tokenizer.hpp"
#include "Basedtest/Suite.hpp"

#include <format>
#include <string>
#include <string_view>

#include "Basedlib/Container/StaticVector.hpp"

// microslop moment
#ifdef __INTELLISENSE__
#pragma diag_suppress 304
#pragma diag_suppress 1574
#pragma diag_suppress 3133
#endif

using namespace Basedtest;
using namespace BasedVT;

using Tokens = Basedlib::StaticVector<Token, 8>;
using Mode = Tokenizer::Mode;

#ifdef BASEDVT_DEBUG
namespace Basedlib {
std::string to_string (const Tokens& tokens) {
	if (tokens.empty()) return {"[empty]"};
	std::string str = {};
	for (std::size_t i = 0; i < tokens.size() - 1; i++)
		str.append (std::format("[{}], ", tokens[i].to_string()));
	str.append (std::format("[{}]", tokens.back().to_string()));
	return str;
}
}
#endif

Tokenizer tokenizer (Mode::STRICT);

template <Mode mode, bool flush = false>
Tokens tokenize (std::string_view str) {
	tokenizer.switch_mode (mode);
	auto tokens = Tokens (tokenizer.feed_string (str));
	if constexpr (flush) {
		if (OptToken flushToken = tokenizer.flush())
			tokens.emplace_back (std::move (*flushToken));
	}
	tokenizer.reset ();
	return tokens;
}

template <typename... Args>
constexpr auto make_test_token_case (std::string_view name, std::string_view input, Args... args) {
	return ValueCase {name, input, Tokens {std::forward<Args>(args)...}};
}

BT_SCENARIO_TEST (test_token_print) {
	BT_ASSERT_RC (Suite ("PRINT", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("a", "a", Token {Token::Type::PRINT, 'a'}),
		make_test_token_case ("[", "[", Token {Token::Type::PRINT, '['}),
		make_test_token_case ("O", "O", Token {Token::Type::PRINT, 'O'}),
		make_test_token_case ("/", "/", Token {Token::Type::PRINT, '/'}),
		make_test_token_case (";", ";", Token {Token::Type::PRINT, ';'}),
		make_test_token_case (":", ":", Token {Token::Type::PRINT, ':'}),
		make_test_token_case ("?", "?", Token {Token::Type::PRINT, '?'}),
		make_test_token_case ("~", "~", Token {Token::Type::PRINT, '~'}),
		make_test_token_case ("[1;2~", "[1;2~",
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
	BT_ASSERT_RC (Suite ("EXEC", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("CR",  "\r",   Token {Token::Type::EXEC, '\r'}),
		make_test_token_case ("LF",  "\n",   Token {Token::Type::EXEC, '\n'}),
		make_test_token_case ("TAB", "\t",   Token {Token::Type::EXEC, '\t'}),
		make_test_token_case ("DEL", "\x7f", Token {Token::Type::EXEC, 0x7F})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_esc) {
	BT_ASSERT_RC (Suite ("ESC", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("ESC",     "\e"),
		make_test_token_case ("ESC ESC", "\e\e"),
		make_test_token_case ("ESC A",   "\eA",  Token {Token::Type::ESC, 'A'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("ESC flush", cases <input_as_constref<tokenize<Mode::STRICT, true>>> (
		make_test_token_case ("ESC",     "\e",   Token {Token::Type::ESC}),
		make_test_token_case ("ESC ESC", "\e\e", Token {Token::Type::ESC}),
		make_test_token_case ("ESC A",   "\eA",  Token {Token::Type::ESC, 'A'})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_esc_inter) {
	BT_ASSERT_RC (Suite ("ESC Inter", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("ESC SP",   "\e "),
		make_test_token_case ("ESC SP A", "\e A",  Token {Token::Type::ESC, 'A', {}, 0, {' '}}),
		make_test_token_case ("ESC !/ B", "\e!/B", Token {Token::Type::ESC, 'B', {}, 0, {'!', '/'}})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_simple) {
	BT_ASSERT_RC (Suite ("CSI simple", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("CSI",         "\e["),
		make_test_token_case ("CSI CSI",     "\e[\e["),
		make_test_token_case ("CSI A",       "\e[A",       Token {Token::Type::CSI, 'A'}),
		make_test_token_case ("CSI DEL A",   "\e[\x7f""A", Token {Token::Type::CSI, 'A'}),
		make_test_token_case ("ESC DEL [ A", "\e\x7f[A",   Token {Token::Type::CSI, 'A'}),
		make_test_token_case ("ESC CSI A",   "\e\e[A",     Token {Token::Type::CSI, 'A'}),
		make_test_token_case ("CSI A CSI B", "\e[A\e[B",   Token {Token::Type::CSI, 'A'}, Token {Token::Type::CSI, 'B'}),
		make_test_token_case ("CSI 1 CSI B", "\e[1\e[B",   Token {Token::Type::CSI, 'B'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("CSI simple flush", cases <input_as_constref<tokenize<Mode::STRICT, true>>> (
		make_test_token_case ("CSI",         "\e["),
		make_test_token_case ("CSI A ESC",   "\e[A\e",     Token {Token::Type::CSI, 'A'}, Token {Token::Type::ESC})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_param) {
	BT_ASSERT_RC (Suite ("CSI param", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("CSI 1;",   "\e[1;"),
		make_test_token_case ("CSI 3~",   "\e[3~",   Token {Token::Type::CSI, '~', {3}}),
		make_test_token_case ("CSI 24~",  "\e[24~",  Token {Token::Type::CSI, '~', {24}}),
		make_test_token_case ("CSI 1;5A", "\e[1;5A", Token {Token::Type::CSI, 'A', {1, 5}})
	)).run_rc());
	BT_ASSERT_RC (Suite ("CSI param flush", cases <input_as_constref<tokenize<Mode::STRICT, true>>> (
		make_test_token_case ("CSI 1;",   "\e[1;")
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_param_wrong) {
	BT_ASSERT_RC (Suite ("CSI param wrong", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("CSI :A",  "\e[:A"),
		make_test_token_case ("CSI 1:A", "\e[1:A")
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_private) {
	BT_ASSERT_RC (Suite ("CSI private marker", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("CSI ?25h", "\e[?25h", Token {Token::Type::CSI, 'h', {25}, '?'}),
		make_test_token_case ("CSI ?25l", "\e[?25l", Token {Token::Type::CSI, 'l', {25}, '?'})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_csi_inter) {
	BT_ASSERT_RC (Suite ("CSI inter", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("CSI ! 1",       "\e[!1"),
		make_test_token_case ("CSI SP ;",      "\e[ ;"),
		make_test_token_case ("CSI ! p",       "\e[!p",     Token {Token::Type::CSI, 'p', {}, 0, {'!'}}),
		make_test_token_case ("CSI 1 ! Q",     "\e[1!Q",    Token {Token::Type::CSI, 'Q', {1}, 0, {'!'}}),
		make_test_token_case ("CSI 12;5 SP R", "\e[12;5 R", Token {Token::Type::CSI, 'R', {12, 5}, 0, {' '}})
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_ss3) {
	BT_ASSERT_RC (Suite ("SS3", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("SS3", "\eO"),
		make_test_token_case ("SS3 A", "\eOA", Token {Token::Type::SS3, 'A'}),
		make_test_token_case ("SS3 B", "\eOB", Token {Token::Type::SS3, 'B'}),
		make_test_token_case ("SS3 R", "\eOR", Token {Token::Type::SS3, 'R'}),
		make_test_token_case ("SS3 S", "\eOS", Token {Token::Type::SS3, 'S'})
	)).run_rc());
	BT_ASSERT_RC (Suite ("SS3 flush", cases <input_as_constref<tokenize<Mode::STRICT, true>>> (
		make_test_token_case ("SS3", "\eO")
	)).run_rc());
	BT_SUCCESS;
}

BT_SCENARIO_TEST (test_token_mix) {
	BT_ASSERT_RC (Suite ("Mixed", cases <input_as_constref<tokenize<Mode::STRICT>>> (
		make_test_token_case ("a CSI 1;5A CR", "a\e[1;5A\r",
			Token {Token::Type::PRINT, 'a'},
			Token {Token::Type::CSI, 'A', {1, 5}},
			Token {Token::Type::EXEC, '\r'}
		)
	)).run_rc());
	BT_SUCCESS;
}

int main () {
	return Suite ("Token", tests (
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
		BT_SUITE_SCENARIO (test_token_mix)
	)).run_rc();
}
