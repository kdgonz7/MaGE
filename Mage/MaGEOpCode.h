#include "MaGEInstructions.h"
#include <map>
#include <unordered_map>
#include <cstdint>
#include <string>

namespace MaGE {
	bool isValidOpcode(uint32_t opcode);
	std::string opcodeAsString(uint32_t opcode);
}
