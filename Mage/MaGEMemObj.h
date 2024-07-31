#pragma once

#include <iostream>
#include <fstream>

#include "MaGE.h"

namespace MaGE
{
	// represents a memory object when called by ask(`size`);
	// .good - is the memory object valid? you can use that as a replacement for NULL checks
	// .offset - where is the memory object located?
	// .size - how big is this memory object?
	// .memory - pointer to the start of the this memory object
	// .code - the return code, under every valid circumstance this should be 0
	class GE_MemoryObject {
	private:
		bool good = true;
		size_t offset = 0;
		size_t size = 0;
		char* memory = nullptr;
		int code = 0;
	public:
		GE_MemoryObject() {}
		GE_MemoryObject(size_t offset, size_t size) : offset(offset), size(size) {}
		~GE_MemoryObject() { }

//-- get functions
		bool 		isGood() { return good; }
		char* 	getMemory() { return memory; }
		size_t 	getOffset() { return offset; }
		size_t 	getSize() { return size; }
		int 		returnCode() { return code; }
		void* 	self() { return this->memory; }

//-- set functions
		void success() { good = true; }
		void fail() { good = false; }
		void setCode(int code) { this->code = code; }
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