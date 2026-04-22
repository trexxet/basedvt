#include "BasedVT/Decode.hpp"

namespace BasedVT {

using namespace InputEvent;

OptEvent decode (const Token& t) {
	if (OptKeyInput input = decode_key (t))
		return Event (std::move (*input));
	return std::nullopt;
}

}
