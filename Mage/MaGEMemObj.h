#pragma once

#include <iostream>
#include <fstream>

#include "MaGE.h"

namespace MaGE
{
	class GE_MemoryObject {
	private:
		bool good = true;
		size_t offset = 0;
		size_t size = 0;
		char* memory = nullptr;
	public:
		GE_MemoryObject() {}
		GE_MemoryObject(size_t offset, size_t size) : offset(offset), size(size) {}

		bool isGood() { return good; }

		char* getMemory() { return memory; }

		size_t getOffset() { return offset; }
		size_t getSize() { return size; }
		void* self() { return this->memory; }

		void success() { good = true; }
		void fail() { good = false; }
		void setOffset(size_t offset) { this->offset = offset; }
		void setSize(size_t size) { this->size = size; }
		void setMemory(char* memory) {
			if (memory == nullptr) {
				good = false;
				return;
			}
			this->memory = memory;
		}
	};
}