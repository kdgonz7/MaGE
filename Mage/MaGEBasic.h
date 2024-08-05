#pragma once

#include "MaGEInstructions.h"

#include <cstdint>
#include <vector>

// Basic representations of bytecode
/* MaGE will use a simple bytecode representation in the form of instructions */

// format; [instruction], [parameters]
namespace MaGE {
	class MaGEInstruction {
		private:
			uint32_t m_instruction;
			std::vector<uint32_t> m_params;
		public:
			MaGEInstruction();
			MaGEInstruction(uint32_t instruction, std::vector<uint32_t> params);

			uint32_t getInstruction() const;
			std::vector<uint32_t> getParams() const;

			~MaGEInstruction();
	};

	class MaGEIR {
		private:
			std::vector<MaGEInstruction> m_instructions;
			uint64_t m_cur = 0;
			bool finished = false;
		public:
			MaGEIR();
			MaGEIR(std::vector<MaGEInstruction> instructions);
			~MaGEIR();

			void pushInstruction(MaGEInstruction instruction);	// pushes an instruction to the instruction stack
			void seek(const size_t& point);
			bool next(); // moves to the next instruction in the stack
			MaGEInstruction* unpack(); // returns if the current instruction is valid
			
	};

	MaGEInstruction createInstruction(uint32_t instruction, std::vector<uint32_t> params);
}

