#include "MaGEEnv.h"
#include "MaGEBasic.h"

using MaGE::MaGEIR;

MaGEIR MaGE::MaGEExecutor::getIR() {
	return this->instruction_repository;
}
