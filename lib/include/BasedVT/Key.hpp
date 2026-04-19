#pragma once

#include <optional>

#include "Basedlib/Class.hpp"
#include "Basedlib/Meta/PrettyEnum.hpp"

#ifdef BASEDVT_DEBUG
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

	char ch = 0;

	bool ctrl = false;
	bool alt = false;
	bool shift = false;

#ifdef BASEDVT_DEBUG
	std::string to_string() const noexcept {
		std::string str = std::format ("Key: {} ch: {}", PrettyKey::to_string (key), static_cast<int> (ch));
		if (ctrl) str.append (" ctrl");
		if (alt) str.append (" alt");
		if (shift) str.append (" shift");
		return str;
	}
#endif

	BASED_CLASS_DEFAULT_EQUALITY (KeyInput);
};

using OptKeyInput = std::optional<KeyInput>;

#ifdef BASEDVT_DEBUG
inline std::string_view to_string (KeyInput::Key k) {
	return KeyInput::PrettyKey::to_string (k);
}
#endif

}
