#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>

#include "MaGEPage.h"
#include "MaGEEndian.h"
#include "MaGEMemObj.h"
#include "MaGE.h"

void* MaGE::GE_Page::allocate()
{
	if (this->data != nullptr) {
		std::cerr << "[MaGE] error: allocate() failed, page already allocated." << std::endl;
		return nullptr;
	}

	if (this->max_size == 0) {
		std::cerr << "[MaGE] error: allocate() failed, no size set." << std::endl;
		return nullptr;
	}

	if (this->max_size < 0) {
		std::cout << "[MaGE] warning: tried to allocate a loaded page." << std::endl;
		return nullptr;
	}

	this->data = new char[this->max_size];
	std::fill(this->data, this->data + this->max_size, MAGE_DEFAULT_UNINITIALIZED);
	
	// note: this is the header for a MaGE file
	// we are writing the header, therefore we don't want to offset by MAGE_HEADERSIZE
	if (this->should_have_authority) {
		writeInt32(0, MAGE_HEADER_PART_1_LOCKED, MaGE::ENDIAN_LITTLE, false);
	}
	else {
		writeInt32(0, MAGE_HEADER_PART_1, MaGE::ENDIAN_LITTLE, false);
	}

	writeInt32(4, MAGE_HEADER_PART_2, MaGE::ENDIAN_LITTLE, false);
	writeInt32(8, MAGE_HEADER_PART_3, MaGE::ENDIAN_LITTLE, false);
	writeInt32(12, MAGE_HEADER_VERSION, MaGE::ENDIAN_LITTLE, false);

	/* integrity check :) */
	int32_t c1 = this->readInt32(0, MaGE::ENDIAN_LITTLE, false);
	int32_t c2 = this->readInt32(4, MaGE::ENDIAN_LITTLE, false);
	int32_t c3 = this->readInt32(8, MaGE::ENDIAN_LITTLE, false);
	
	if ((c1 != MAGE_HEADER_PART_1 and c1 != MAGE_HEADER_PART_1_LOCKED) or c2 != MAGE_HEADER_PART_2 or c3 != MAGE_HEADER_PART_3) {
		std::cerr << "[MaGE] error: allocate() failed, malformed header" << std::endl;
		return nullptr;
	}

	return this->data;
}

// again, write data to the offset address
// skip_header - should this function instead offset from 0, or from the length of the header (prevents corruption)
void MaGE::GE_Page::write(const char* data, const size_t size, const size_t offset, bool skip_header) {
	// if our data is just wrong
	if (this->data == nullptr or offset >= this->max_size or offset + size >= this->max_size) {
		std::cerr << "[MaGE] error: write() failed, no page allocated." << std::endl;
		return;
	}

	/* we copy the data to the offset */
	memcpy(this->data + (offset + (skip_header ? MAGE_HEADERSIZE : 0)), data, size);
}

// writes data where data is free.
// this function can not guarantee that the entire length 
// of the data is free, only the first free spot.
void MaGE::GE_Page::writeWhereFree(const char* info, const size_t size)
{
	for (size_t i = 0; i < this->max_size; i++) {
		if (this->data[i] == -1) {
			this->write(info, size, i);
			return;
		}
		else {
			std::cout << "[MaGE] warning: writeWhereFree() failed, no available space" << std::endl;
			return;
		}
	}
}

// dumps all of the data to a file, does not lock the file
void MaGE::GE_Page::dumpToFile(const char* filename)
{
	std::ofstream data_file(filename, std::ios::out | std::ios::binary);

	/* if we couldn't open the file, i guess something went wrong */
	if (!data_file.is_open()) {
		std::cerr << "[MaGE] error: dumpToFile() failed, cannot open file." << std::endl;
		return;
	}

	// we write the data
	data_file.write(this->data, this->max_size);
	data_file.close();
}

// loads the data from file "filename"
// this function will load the file, as well
// as ensure the file is a valid MaGE file
void MaGE::GE_Page::load(const char* filename)
{
	this->filename = filename;

	if (this->should_have_authority) { // if we should have authority, lock the file
		if (!MaGE::simpleLock(filename)) {
			std::cerr << "[MaGE] error: loadFromDefault() failed, file is locked. @protected" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	std::ifstream data_file(filename, std::ios::in | std::ios::binary);

	data_file.seekg(0, data_file.end);	// seek to the end then back
	size_t size = data_file.tellg();	// get the size
	data_file.seekg(0, data_file.beg);	// we now have the size

	// we set the size to this->max_size
	this->max_size = size;
	this->data = new char[size];

	if (this->data == nullptr) {
		std::cerr << "[MaGE] error: loadFromDefault failed to allocate memory." << std::endl;
		return;
	}

	if (! data_file.is_open()) {
		std::cerr << "[MaGE] error: loadFromDefault() failed, cannot open file." << std::endl;
		return;
	}

	// read the data file into the page
	// ensuring not more than the file size is read
	data_file.read(this->data, this->max_size); // read the data into the page
	data_file.close();

	// check for the MaGE header
	if (!this->dataStreamHasMagic(0)) {
		data_file.close();
		std::cerr << "[MaGE] error: loadFromDefault() failed, file is not a MaGE file." << std::endl;
		exit(-1);
	}

	int32_t h1 = this->readInt32(0, MaGE::ENDIAN_LITTLE, false);

	if (h1 == MAGE_HEADER_PART_1_LOCKED and !this->should_have_authority) {
		std::cerr << "[MaGE] error: load failed, function not called with authority" << std::endl;
		exit(-1);
	}
	// read the version
	// [1]... [2]... [3]... [VERSION]
	int32_t version = this->readInt32(12, MaGE::ENDIAN_LITTLE, false);

	// if the version is outdated
	if (version != MAGE_HEADER_VERSION) {
		std::cerr << "[MaGE] error: header version is outdated." << std::endl;
		exit(-1);
	}

	// set the version to be accessed by other public functions
	this->version = version;
}

// reads an int32 from the data stream, dumps its bytes to a temporary buffer
// then returns the int32 as a pointer.
//! we need skip_header for finding out header matrix
int32_t MaGE::GE_Page::readInt32(const size_t offset, const MaGE::Endian endianness, bool skip_header)
{
	char tmp[4];
	
	memcpy(tmp, this->data + (offset + (skip_header ? MAGE_HEADERSIZE : 0)), 4);

	if (endianness == MaGE::ENDIAN_BIG) {
		std::reverse(tmp, tmp + 4);
	}
	
	return *(int32_t*)tmp;
}

// reads an int64 from the data stream
// uses the same strategy as readInt32.
// todo: probably need to add bit shifting for portability
int64_t MaGE::GE_Page::readInt64(const size_t offset, MaGE::Endian endianness) const
{
	char tmp[8];
	
	memcpy(tmp, this->data + offset, 8);
	
	if (endianness == MaGE::ENDIAN_BIG) {
		std::reverse(tmp, tmp + 8);
	}
	
	return *(int64_t*)tmp;
}

// writes an int32 to the data stream.
// this function starts from the offset, 
// and writes the int32 to that offset (4 byte alignment)
void MaGE::GE_Page::writeInt32(const size_t offset, const int32_t value, MaGE::Endian endianness, bool skip_header)
{
	char tmp[4];
	
	memcpy(tmp, &value, 4);
	
	if (endianness == MaGE::ENDIAN_BIG) {
		std::reverse(tmp, tmp + 4);
	}
	
	memcpy(this->data + (offset + (skip_header ? MAGE_HEADERSIZE : 0)), tmp, 4);
}

// writes an int64 to the data stream
// similar to int32
void MaGE::GE_Page::writeInt64(const size_t offset, const int64_t value, MaGE::Endian endianness)
{
	char tmpbuf[8];

	// we copy the value into a sorta buffer
	// so we can get the raw bytes
	memcpy(tmpbuf, &value, 8);
		
	// reverse if big endian
	if (endianness == MaGE::ENDIAN_BIG) {
		std::reverse(tmpbuf, tmpbuf + 8);
	}
	
	// we add the data to the page
	memcpy(this->data + (offset + MAGE_HEADERSIZE), tmpbuf, 8);
}

// checks if the data stream has the MaGE header
// uses macros, therefore should stay up to date, as long
// as this code remains barely touched
bool MaGE::GE_Page::dataStreamHasMagic(const char* data)
{
	size_t sizeOfMagic = sizeof(int32_t) * 4;

	if (sizeOfMagic > this->max_size) {
		std::cerr << "[MaGE] error: dataStreamHasMagic() failed, data stream too small." << std::endl;
		return false;
	}
 
	// checks if the data stream has the MaGE header.
	int32_t c1 = this->readInt32(0, MaGE::ENDIAN_LITTLE, false);
	int32_t c2 = this->readInt32(4, MaGE::ENDIAN_LITTLE, false);
	int32_t c3 = this->readInt32(8, MaGE::ENDIAN_LITTLE, false);
	
	return ((c1 == MAGE_HEADER_PART_1 or c1 == MAGE_HEADER_PART_1_LOCKED) and c2 == MAGE_HEADER_PART_2 and c3 == MAGE_HEADER_PART_3);
}

// returns a pointer to a free memory block
// todo: make size actually useful
MaGE::GE_MemoryObject* MaGE::GE_Page::ask(size_t size)
{
	using MaGE::GE_MemoryObject;

	GE_MemoryObject* obj = new GE_MemoryObject();

	if (obj == nullptr) {
		return nullptr;
	}

	if (size >= this->max_size) {
		obj->fail();
		obj->setCode(EMEMORYOVERFLOW);

		return obj;
	}

	// we start at 0.
	size_t offset = static_cast<size_t>(0) + MAGE_HEADERSIZE;

	// search for a free (?) area with uninitialized data
	for (size_t i = offset ; i < this->max_size ; i++) {
		if (this->data[i] == -1) {
			offset = i;

			for (size_t j = i ; j < i + size ; j++) {
				this->data[j] = 0;
			} // set the memory to uninitialized

			break;
		}
	}

	// we set the pointer to the free (? area)
	char* tbuf = this->data + offset; // we use data from a free offset.

	// if it's null, then we fail
	if (tbuf == nullptr) {
		obj->fail();
		return obj;
	}

	// set the memory to the pointer, and success, set the offset and size
	obj->setMemory(tbuf);
	obj->success();
	obj->setOffset(offset + 1);
	obj->setSize(size);

	return obj;
}

// sets if the page should have authority,
// this should be set before calling loadFromDefault
void MaGE::GE_Page::setAuthority(bool info)
{
	this->should_have_authority = info;
}

// unlocks the file and cleans up the memory used
// should be called when the page is no longer needed (in authority mode)
void MaGE::GE_Page::endUsage()
{
	if (!this->filename) {
		std::cerr << "[MaGE] error: endUsage() failed: this is not a read page" << std::endl;
		return;
	}

	MaGE::unLock(this->filename);

	this->filename = nullptr;
	this->should_have_authority = false;

	if (this->data != nullptr) {
		delete[] this->data;
		this->data = nullptr;
	}

	this->max_size = 0;

	// it's cool to delete the object now
}
