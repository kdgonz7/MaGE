#include "MaGEState.h"

#include <iostream>

bool MaGE::MaGEState::set(MaGE::M_State new_state) {
	// if we aren't trapped, don't do anything
	if (new_state == S_RETURNFROMTRAP and this->m_state != S_TRAP) {
		std::cerr << "[MaGE] error: set() failed, not trapped." << std::endl;
		return false;
	}

	if (new_state == S_SLEEPING and this->m_state != S_INSTRUCTION) {
		std::cerr << "[MaGE] warn: set() called in potential trap, ignoring" << std::endl;
		return false;
	}

	this->m_state = new_state;
	
	return true;
}