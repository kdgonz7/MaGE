#include <unordered_map>

#include "MaGEOpCode.h"
#include "MaGEInstructions.h"

namespace MaGE
{
	// define instructions here
	std::unordered_map<MaGE::MaGEInstructionSet, std::string> opcode_names = {
		{MaGE::MaGEInstructionSet::EARLY, "EARLY"},
		{MaGE::MaGEInstructionSet::HALT, "HALT"},
		{MaGE::MaGEInstructionSet::STORE, "STORE"},
		{MaGE::MaGEInstructionSet::LOAD, "LOAD"},
	};
}

bool MaGE::isValidOpcode(uint32_t opcode)
{

	if (opcode_names.find(MaGE::MaGEInstructionSet(opcode)) != opcode_names.end())
	{
		return true;
	}

	return false;
}

std::string MaGE::opcodeAsString(uint32_t opcode) {
	try {
		auto name = opcode_names.at(MaGE::MaGEInstructionSet(opcode));
		return name;
	} catch (...) {
		return "INVALID";
	}
}
