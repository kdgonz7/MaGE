#include "MaGELoad.h"

#include <fstream>
#include <iostream>
#include <cstdint>

uint32_t* MaGE::loadf(std::string filename) {
	std::fstream exefile (filename, std::ios::in | std::ios::binary);

	if (!exefile.is_open()) {
		std::cerr << "[MaGE] error: loadf() failed, cannot open file." << std::endl;
		return nullptr;
	}

	exefile.seekg(0, exefile.end);
	size_t size = exefile.tellg();
	exefile.seekg(0, exefile.beg);

	uint32_t* data = new uint32_t[size];

	if (data == nullptr) {
		std::cerr << "[MaGE] error: loadf() failed to allocate memory." << std::endl;
		return nullptr;
	}

	exefile.read((char*)data, size);

	return data;
}

// if i'm being #real, i might
// delete these later

/* these are for reading bytecode files */

uint32_t MaGE::byteinto32(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
	uint32_t result = 0;

	result = b1;

	result = result << 8;
	result = result | b2;
	result = result << 8;
	result = result | b3;
	result = result << 8;
	result = result | b4;

	return result;
}

uint64_t MaGE::byteinto64(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8) {
	uint64_t result = 0;

	result = b1;

	result = result << 8;
	result = result | b2;
	result = result << 8;
	result = result | b3;
	result = result << 8;
	result = result | b4;
	result = result << 8;
	result = result | b5;
	result = result << 8;
	result = result | b6;
	result = result << 8;
	result = result | b7;
	result = result << 8;
	result = result | b8;

	return result;
}
