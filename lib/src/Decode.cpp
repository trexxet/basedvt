#include "Decode.hpp"

namespace BasedVT {

std::optional<KeyInput> decode (const Token& t) {
	switch (t.type) {
		case Token::Type::PRINT:
			break;
		case Token::Type::EXEC:
			break;
		case Token::Type::ESC:
			break;
		case Token::Type::CSI:
			break;
	}
	return std::nullopt;
}

}
