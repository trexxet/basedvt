#pragma once

#include "BasedVT/InputEvent/Event.hpp"
#include "BasedVT/InputEvent/Key.hpp"
#include "BasedVT/Token.hpp"

namespace BasedVT {

InputEvent::OptKeyInput decode_key (const Token& t);

InputEvent::OptEvent decode (const Token& t);

}
