#include "FSMActions.hpp"

#include <array>

#include "Basedlib/PrettyEnum.hpp"

namespace BasedVT {

using Action = void (*) (FSMDetail::Context*);

namespace Actions {

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

}

using PrettyActions = Basedlib::PrettyEnum<FSMDetail::Actions>;

constexpr std::array <Action, PrettyActions::size> actions = {
	nullptr,
	Actions::print,
	Actions::execute,
	Actions::clear,
	Actions::collect,
	Actions::param,
	Actions::esc_dispatch,
	Actions::csi_dispatch
};

void action (FSMDetail::Actions ac, FSMDetail::Context* ctx) {
	actions[PrettyActions::idx(ac)] (ctx);
}

}
