#pragma once

#ifdef BASEDVT_DEBUG
#include <format>
#include <string>
#include "Basedlib/PrettyEnum.hpp"
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

	char ch = 0;

	bool shift = false;
	bool ctrl = false;
	bool alt = false;

#ifdef BASEDVT_DEBUG
	const std::string to_string() const noexcept {
		return std::format ("Key: {} ch: {} shift = {} ctrl = {} alt = {}",
			Basedlib::PrettyEnum<Key>::to_string (key),
			static_cast<int> (ch),
			shift, ctrl, alt
		);
	}
#endif
};

}
