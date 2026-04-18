#include "FSMActions.hpp"

#include <array>

#include "Basedlib/Meta/PrettyEnum.hpp"
#include "BasedVT/Token.hpp"

#ifdef BASEDVT_DEBUG
#include <print>
#endif

namespace BasedVT {

using namespace FSMDetail;

inline static void ready (Context* ctx, Token::Type type) {
	ctx->stage.commit_ch();
	ctx->stage.set_type (type);
	ctx->ready = std::move (ctx->stage.ready());

#ifdef BASEDVT_DEBUG
	std::print ("Token: {}\n", ctx->ready->to_string());
#endif
}

void print (Context* ctx) {
	ready (ctx, Token::Type::PRINT);
}

void execute (Context* ctx) {
	const uint8_t b = ctx->stage.currByte;

	ctx->ready.emplace (Token::Type::EXEC, ctx->stage.currByte);

	if (b == 0x18 || b == 0x1A)
		ctx->stage = {};

#ifdef BASEDVT_DEBUG
	std::print ("Token: {}\n", ctx->ready->to_string());
#endif
}

void clear (Context* ctx) {
	ctx->stage = {};
	ctx->ready = {};
}

void collect (Context* ctx) {
	const uint8_t b = ctx->stage.currByte;

	if (b >= '>' && b <= '?') {
		ctx->stage.commit_private();
		return;
	}

	ctx->stage.commit_intermediate();
}

void param (Context* ctx) {
	const uint8_t b = ctx->stage.currByte;

	if (b == ';') {
		ctx->stage.commit_param();
		return;
	}

	ctx->stage.append_param();
}

void esc_dispatch (Context* ctx) {
	ready (ctx, Token::Type::ESC);
}

void csi_dispatch (Context* ctx) {
	if (ctx->stage.currParam >= 0)
		ctx->stage.commit_param();

	ready (ctx, Token::Type::CSI);
}

void ss3_dispatch (Context *ctx) {
	ready (ctx, Token::Type::SS3);
}

using PrettyActions = Basedlib::PrettyEnum <Actions>;

constexpr std::array actions = {
	print,
	execute,
	clear,
	collect,
	param,
	esc_dispatch,
	csi_dispatch,
	ss3_dispatch
};

void action (Actions ac, Context* ctx) {
#ifdef BASEDVT_DEBUG
	std::print ("Action: {}\n", PrettyActions::to_string(ac));
#endif
	actions[PrettyActions::idx(ac)] (ctx);
}

}
