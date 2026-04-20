#include "FSMClassifier.hpp"

#include <array>
#include <cstdint>

#include "Basedlib/Meta/PrettyEnum.hpp"

#define SS3_ENTRY 0x4F
#define CSI_ENTRY 0x5B
#define DEL 0x7F
#define SS3_HIGH 0x8F
#define CSI_HIGH 0x9B

#define RANGE_PRINTABLE    0x20 ... 0x7E
#define RANGE_INTERMEDIATE 0x20 ... 0x2F
#define RANGE_HIGH_ASCII   0x80 ... 0xFF

namespace BasedVT {

using namespace FSMDetail;

OptEvent classify_c0 (uint8_t b) noexcept {
	switch (b) {
		case 0x00 ... 0x17:
		case 0x19:
		case 0x1C ... 0x1F: return Events::EV_EXECUTE;
		case 0x18:
		case 0x1A:          return Events::EV_EXECUTE_CANCEL;
		case 0x1B:          return Events::EV_ESC;
	}
	return std::nullopt;
}

OptEvent classify_c1_strict (uint8_t b) noexcept {
	switch (b) {
		case 0x80 ... 0x8E:
		case 0x91 ... 0x97:
		case 0x99:
		case 0x9A:     return Events::EV_EXECUTE_CANCEL;
		case SS3_HIGH: return Events::EV_SS3_HIGH;
		case CSI_HIGH: return Events::EV_CSI_HIGH;
		/* DCS */ case 0x90:
		/* SOS */ case 0x98:
		/* ST  */ case 0x9C:
		/* OSC */ case 0x9D:
		/* PM  */ case 0x9E:
		/* APC */ case 0x9F: return Events::EV_IGNORE;
	}
	return std::nullopt;
}

OptEvent classifier_ground (uint8_t b) noexcept {
	switch (b) {
		case RANGE_HIGH_ASCII:
		[[likely]]
		case RANGE_PRINTABLE: return Events::EV_PRINTABLE;
		case DEL:             return Events::EV_EXECUTE;
	}
	return std::nullopt;
}

OptEvent classifier_esc (uint8_t b) noexcept {
	switch (b) {
		[[likely]]
		case CSI_ENTRY:          return Events::EV_CSI_ENTRY;
		case SS3_ENTRY:          return Events::EV_SS3_ENTRY;
		case RANGE_INTERMEDIATE: return Events::EV_INTERMEDIATE;
	};
	switch (b) {
		case RANGE_PRINTABLE:    return Events::EV_FINAL;
	};
	return std::nullopt;
}

OptEvent classifier_esc_inter (uint8_t b) noexcept {
	switch (b) {
		case RANGE_INTERMEDIATE: return Events::EV_INTERMEDIATE;
	};
	switch (b) {
		case RANGE_PRINTABLE:    return Events::EV_FINAL;
	};
	return std::nullopt;
}

OptEvent classifier_csi (uint8_t b) noexcept {
	switch (b) {
		case RANGE_INTERMEDIATE: return Events::EV_INTERMEDIATE;
		case 0x30 ... 0x39:
		case 0x3B:               return Events::EV_PARAM;
		case 0x3A:               return Events::EV_COLON;
		case 0x3C ... 0x3F:      return Events::EV_PRIVATE;
		case 0x40 ... 0x7E:      return Events::EV_FINAL;
	};
	return std::nullopt;
}

OptEvent classifier_ss3 (uint8_t b) noexcept {
	switch (b) {
		[[likely]]
		case RANGE_PRINTABLE: return Events::EV_FINAL;
	};
	return std::nullopt;
}

constexpr std::array classifiers = {
	classifier_ground,
	classifier_esc,
	classifier_esc_inter,
	classifier_csi,
	classifier_csi,
	classifier_csi,
	classifier_csi,
	classifier_ss3
};

OptEvent classify_byte (uint8_t b, FSMDetail::Mode mode, FSMDetail::States state) {
	OptEvent optEvent = classify_c0 (b);
	if (optEvent) return optEvent;
	if (mode == FSMDetail::Mode::STRICT) {
		optEvent = classify_c1_strict (b);
		if (optEvent) return optEvent;
	}

	using PrettyState = Basedlib::PrettyEnum<FSMDetail::States>;
	return classifiers [PrettyState::idx (state)] (b);
}

}
