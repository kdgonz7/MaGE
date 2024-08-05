#include <fstream>
#include <cstdint>

namespace MaGE
{
	/* loads a 32-bit MaGE instruction file into memory*/

	uint32_t* loadf(std::string filename);
	uint32_t	byteinto32(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
	uint64_t	byteinto64(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8);
}
