#pragma once

#include <optional>
#include <variant>

#include "BasedVT/InputEvent/Key.hpp"

namespace BasedVT::InputEvent {

using Event = std::variant <
	KeyInput
>;

using OptEvent = std::optional <Event>;

}
