#pragma once

#include <array>
#include <cstdint>
#include <bit>

namespace ccrc32 {
// TODO: add documentation
class CRC32 {
	static constexpr std::array<uint32_t, 256> lut = [] {
		std::array<uint32_t, 256> res;
		for (std::size_t i = 0; i < res.size(); i++) {
			res[i] = i;
			for (int j = 0; j < 8; j++) {
				res[i] = res[i] & 1 ? (res[i] >> 1) ^ 0xEDB88320U : res[i] >> 1;
			}
		}
		return res;
	}();
	uint32_t crc32 = 0xFFFFFFFF;
	
	public:
	constexpr CRC32() = default;
	
	constexpr uint32_t get() const {
		return ~crc32;
	}
	
	constexpr uint32_t reset() {
		const uint32_t old = ~crc32;
		crc32 = 0xFFFFFFFF;
		return old;
	}
	
	constexpr uint32_t update(const uint8_t byte) {
		crc32 = (crc32 >> 8) ^ lut[(crc32 ^ byte) % lut.size()];
		return ~crc32;
	}
	
	constexpr uint32_t update(const auto *const data, const std::size_t len) {
		for (std::size_t i = 0; i < len; i++) {
			crc32 = (crc32 >> 8) ^ lut[(crc32 ^ std::bit_cast<uint8_t>(data[i])) % lut.size()];
		}
		return ~crc32;
	}
};
}
