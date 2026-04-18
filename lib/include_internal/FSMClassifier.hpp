#pragma once

#include <optional>

#include "BasedVT/FSMDetail.hpp"

namespace BasedVT {

using OptEvent = std::optional <FSMDetail::Events>;

OptEvent classify_byte (uint8_t b, FSMDetail::Mode mode, FSMDetail::States state);

}
