#pragma once

#include <optional>

#include "BasedVT/Key.hpp"
#include "BasedVT/Token.hpp"

namespace BasedVT {

std::optional<KeyInput> decode (const Token& t);

}
