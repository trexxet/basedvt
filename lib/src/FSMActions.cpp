#include "FSMActions.hpp"

#include <array>

#include "Basedlib/PrettyEnum.hpp"

#ifdef BASEDVT_DEBUG
#include <print>
#endif

namespace BasedVT {

using Action = void (*) (FSMDetail::Context*);

void print (FSMDetail::Context*) {

}

void execute (FSMDetail::Context*) {

}

void clear (FSMDetail::Context*) {

}

void collect (FSMDetail::Context*) {

}

void param (FSMDetail::Context*) {

}

void esc_dispatch (FSMDetail::Context*) {

}

void csi_dispatch (FSMDetail::Context*) {

}

using PrettyActions = Basedlib::PrettyEnum<FSMDetail::Actions>;

constexpr std::array <Action, PrettyActions::size> actions = {
	nullptr,
	print,
	execute,
	clear,
	collect,
	param,
	esc_dispatch,
	csi_dispatch
};

void action (FSMDetail::Actions ac, FSMDetail::Context* ctx) {
#ifdef BASEDVT_DEBUG
	std::print ("Action: {}\n", PrettyActions::to_string(ac));
#endif
	actions[PrettyActions::idx(ac)] (ctx);
}

}
