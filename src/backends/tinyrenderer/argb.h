#pragma once

struct ARGB {
	uint8_t& a() { return ((uint8_t*)(&data))[0]; }
	uint8_t& r() { return ((uint8_t*)(&data))[1]; }
	uint8_t& g() { return ((uint8_t*)(&data))[2]; }
	uint8_t& b() { return ((uint8_t*)(&data))[3]; }
private:
	uint32_t data = 0;
};
