#include "FSMClassifier.hpp"

#include <array>
#include <cstdint>

#include "Basedlib/Meta/PrettyEnum.hpp"

#define SS3_ENTRY 0x4F
#define CSI_ENTRY 0x5B
#define DEL 0x7F

namespace BasedVT {

using namespace FSMDetail;

inline bool in_printable_range (uint8_t b) {
	return (b >= 0x20 && b <= 0x7E);
}

inline bool in_intermediate_range (uint8_t b) {
	return (b >= 0x20 && b <= 0x2F);
}

OptEvent classifier_ground (uint8_t b) {
	if (in_printable_range (b)) [[likely]]
		return Events::EV_PRINTABLE;
	if (b == DEL)
		return Events::EV_EXECUTE;
	return std::nullopt;
}

OptEvent classifier_esc (uint8_t b) {
	if (b == CSI_ENTRY) [[likely]]
		return Events::EV_CSI_ENTRY;
	if (b == SS3_ENTRY)
		return Events::EV_SS3_ENTRY;
	if (in_intermediate_range (b))
		return Events::EV_INTERMEDIATE;
	if (in_printable_range (b))
		return Events::EV_FINAL;
	return std::nullopt;
}

OptEvent classifier_esc_inter (uint8_t b) {
	if (in_intermediate_range (b))
		return Events::EV_INTERMEDIATE;
	if (in_printable_range (b))
		return Events::EV_FINAL;
	return std::nullopt;
}

OptEvent classifier_csi (uint8_t b) {
	switch (b) {
		case 0x20 ... 0x2F: return Events::EV_INTERMEDIATE;
		case 0x30 ... 0x39:
		case 0x3B:          return Events::EV_PARAM;
		case 0x3A:          return Events::EV_COLON;
		case 0x3C ... 0x3F: return Events::EV_PRIVATE;
		case 0x40 ... 0x7E: return Events::EV_FINAL;
	};
	return std::nullopt;
}

OptEvent classifier_ss3 (uint8_t b) {
	if (in_printable_range (b)) [[likely]]
		return Events::EV_FINAL;
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

OptEvent classify_byte (uint8_t b, FSMDetail::States state) {
	switch (b) {
		case 0x00 ... 0x17:
		case 0x19:
		case 0x1C ... 0x1F: return Events::EV_EXECUTE;
		case 0x18:
		case 0x1A: return Events::EV_EXECUTE_CANCEL;
		case 0x1B: return Events::EV_ESC;
	};

	using PrettyState = Basedlib::PrettyEnum<FSMDetail::States>;
	return classifiers [PrettyState::idx (state)] (b);
}

}
