#include "MaGEBasic.h"
#include <iostream>

using std::string;

namespace MaGE {
	// the mage environment keeps track of everything, state, etc.
	class MaGEExecutor {
		uint32_t s_out = 0;		// stdout
		uint32_t s_in = 1;		// stdin
		uint32_t s_err = 2; 	// stderr

		MaGEIR instruction_repository = MaGEIR(); // keep track of instructions
	public:
		MaGEExecutor() = default;
		~MaGEExecutor() = default;
		
		uint32_t getStdOut();
		uint32_t getStdIn();
		uint32_t getStdErr();

		MaGEIR getIR();
	};
}
