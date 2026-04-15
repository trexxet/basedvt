#pragma once

#include <optional>

#include "BasedVT/Key.hpp"
#include "BasedVT/Token.hpp"

namespace BasedVT {

using OptKeyInput = std::optional<KeyInput>;

OptKeyInput decode (const Token& t);

}
