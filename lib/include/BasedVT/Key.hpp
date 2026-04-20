#pragma once

#include <cstdint>
#include <optional>

#include "Basedlib/Class.hpp"
#include "Basedlib/Meta/PrettyEnum.hpp"

#ifdef BASEDVT_ENABLE_TO_STRING
#include <format>
#include <string>
#include <string_view>
#endif

namespace BasedVT {

struct KeyInput {
	enum class Key {
		NONE,
		CHAR,
		BACKSPACE,
		TAB,
		ENTER,
		ESCAPE,
		PAGE_UP,
		PAGE_DOWN,
		END,
		HOME,
		LEFT,
		UP,
		RIGHT,
		DOWN,
		INSERT,
		DELETE,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	} key = Key::NONE;
	using PrettyKey = Basedlib::PrettyEnum<Key>;

	uint8_t byte = 0;

	bool ctrl = false;
	bool alt = false;
	bool shift = false;

#ifdef BASEDVT_ENABLE_TO_STRING
	std::string to_string() const noexcept {
		std::string str = std::format ("Key: {} byte: {}", PrettyKey::to_string (key), byte);
		if (ctrl) str.append (" ctrl");
		if (alt) str.append (" alt");
		if (shift) str.append (" shift");
		return str;
	}
#endif

	BASED_CLASS_DEFAULT_EQUALITY (KeyInput);
};

using OptKeyInput = std::optional<KeyInput>;

#ifdef BASEDVT_ENABLE_TO_STRING
inline std::string_view to_string (KeyInput::Key k) {
	return KeyInput::PrettyKey::to_string (k);
}
#endif

}
