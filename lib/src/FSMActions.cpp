#include "FSMActions.hpp"

#include <array>

#include "Basedlib/PrettyEnum.hpp"
#include "BasedVT/Token.hpp"

#ifdef BASEDVT_DEBUG
#include <print>
#endif

namespace BasedVT {

using namespace FSMDetail;

inline static void ready (Context* ctx, Token::Type type) {
	ctx->token.type = type;
	ctx->token.ch = ctx->currByte;
	ctx->ready = true;

#ifdef BASEDVT_DEBUG
	std::print ("Token: {}\n", ctx->token.to_string());
#endif
}

void print (Context* ctx) {
	ready (ctx, Token::Type::PRINT);
}

void execute (Context* ctx) {
	ready (ctx, Token::Type::EXEC);
}

void clear (Context* ctx) {
	*ctx = {};
}

void collect (Context* ctx) {
	const uint8_t b = ctx->currByte;

	if (b >= '>' && b <= '?') {
		ctx->token.privateMark = b;
		return;
	}

	ctx->token.add_intermediate (static_cast<char> (b));
}

void param (Context* ctx) {
	const uint8_t b = ctx->currByte;

	if (b == ';') {
		ctx->token.add_param (&ctx->currParam);
		return;
	}

	if (ctx->currParam < 0) ctx->currParam = 0;
	ctx->currParam = ctx->currParam * 10 + (b - '0');
}

void esc_dispatch (Context* ctx) {
	ready (ctx, Token::Type::ESC);
}

void csi_dispatch (Context* ctx) {
	if (ctx->currParam >= 0)
		ctx->token.add_param (&ctx->currParam);

	ready (ctx, Token::Type::CSI);
}

using PrettyActions = Basedlib::PrettyEnum <Actions>;

constexpr std::array actions = {
	print,
	execute,
	clear,
	collect,
	param,
	esc_dispatch,
	csi_dispatch
};

void action (Actions ac, Context* ctx) {
#ifdef BASEDVT_DEBUG
	std::print ("Action: {}\n", PrettyActions::to_string(ac));
#endif
	actions[PrettyActions::idx(ac)] (ctx);
}

}
