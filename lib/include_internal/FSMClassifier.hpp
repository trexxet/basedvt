#pragma once

#include "BasedVT/FSMDetail.hpp"

namespace BasedVT {

FSMDetail::OptEvent classify_byte (uint8_t b, FSMDetail::Mode mode, FSMDetail::States state);

}
