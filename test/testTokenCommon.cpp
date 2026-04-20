#define BASEDVT_TOKEN_TEST_COMMON
#include "testTokenCommon.hpp"

#ifdef BASEDVT_ENABLE_TO_STRING
namespace Basedlib {
std::string to_string (const Tokens& tokens) {
	if (tokens.empty()) return {"[empty]"};
	std::string str = {};
	for (std::size_t i = 0; i < tokens.size() - 1; i++)
		str.append (std::format("[{}], ", tokens[i].to_string()));
	str.append (std::format("[{}]", tokens.back().to_string()));
	return str;
}
}
#endif
