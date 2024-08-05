#include "MaGEBasic.h"

#include <iostream>

MaGE::MaGEInstruction::MaGEInstruction()
	: m_instruction(0), m_params({}) {}

MaGE::MaGEInstruction::MaGEInstruction(uint32_t instruction, std::vector<uint32_t> params)
	: m_instruction(instruction), m_params(params) {}

uint32_t MaGE::MaGEInstruction::getInstruction() const {
	return this->m_instruction;
}

std::vector<uint32_t> MaGE::MaGEInstruction::getParams() const {
	return this->m_params;
}

MaGE::MaGEInstruction MaGE::createInstruction(uint32_t instruction, std::vector<uint32_t> params) {
	return MaGE::MaGEInstruction(instruction, params);
}

MaGE::MaGEIR::MaGEIR() {
	this->m_instructions.clear();
}

// increments the instruction pointer
// note: returns false if we're at the end
bool MaGE::MaGEIR::next() {
	if (this->m_cur < this->m_instructions.size()) {
		this->m_cur++;
		return true;
	}

	return false;
}

// unpacks the current instruction.
// note: returns an empty instruction if we're at the end
MaGE::MaGEInstruction* MaGE::MaGEIR::unpack() {
	if (this->m_cur >= this->m_instructions.size()) {
		return nullptr;
	}

	return &this->m_instructions.at(this->m_cur);
}

void MaGE::MaGEIR::pushInstruction(MaGE::MaGEInstruction instruction) {
	this->m_instructions.push_back(instruction);
}

void MaGE::MaGEIR::seek(const size_t & point) {
	if (point >= this->m_instructions.size() || point < 0) {
		std::cerr << "[MaGE] bad seek. failed" << std::endl;
		std::cerr << "[MaGE] what(): " << point << std::endl;
	}

	this->m_cur = point;
}

MaGE::MaGEIR::~MaGEIR() = default;
MaGE::MaGEInstruction::~MaGEInstruction() = default;
