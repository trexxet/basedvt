#pragma once

#include "FSMDetail.hpp"

namespace BasedVT {

class Parser {

	FSMDetail::Context ctx;
	FSMDetail::FSM fsm;

public:
	void feed (char c);

	Parser ();
};


}
