#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "MaGESimpleLock.h"
#include "MaGEEndian.h"
#include "MaGEMemObj.h"

/* MaGE Page System */

namespace MaGE {
	class GE_Page {
		const char* page_name;					// the page name
		size_t max_size;						// the maximum size of the page
		bool should_have_authority = false;		// if this page should be locked for exclusive use
		char *data = nullptr;					// the data of the page
		const char* filename = nullptr;			// the filename of the page
		int32_t version = 0;					// the version of the page
	public:
		GE_Page(const char* name, const size_t max_size = 0) : 
			page_name(name), max_size(max_size) {}

		// i guess destructors are called at the end of a scope
		// like when it's done
		// that's pretty freakin cool!!!
		~GE_Page() {
			// clean up
			if (this->data != nullptr) {
				delete[] this->data;
			}
		}

		const char* nameOf() { return page_name;  }
		const size_t sizeOf() { return max_size;  }
		const char* getFilename() { return filename; }
		const int32_t getVersion() { return version; }

		// Allocates a new page with a size of `this.max_size`
		void* allocate();

		// Writes to the data at the offset address. This will overwrite any existing data.
		void write(const char* data, const size_t size, const size_t offset = 0, bool skip_header = true);

		// Writes to the page where the data is 0
		void writeWhereFree(const char* info, const size_t size);

		// Creates a new page file
		void dumpToFile(const char* filename);

		// Loads this page from the specified file
		void load(const char* filename);

		// Reads an integer from the specified offset
		int32_t readInt32(const size_t offset = 0, MaGE::Endian endianness = MaGE::Endian::ENDIAN_LITTLE, bool skip_header = true);
		int64_t readInt64(const size_t offset = 0, MaGE::Endian endianness = MaGE::Endian::ENDIAN_LITTLE) const;

		// Writes an integer to the specified offset
		void writeInt32(const size_t offset, const int32_t value, MaGE::Endian endianness = MaGE::Endian::ENDIAN_LITTLE, bool skip_header = true);
		void writeInt64(const size_t offset, const int64_t value, MaGE::Endian endianness = MaGE::Endian::ENDIAN_LITTLE);

		// Checks if the data stream has the magic number
		bool dataStreamHasMagic(const char* data);

		// Essentially `malloc(size)`
		MaGE::GE_MemoryObject *ask(size_t size);

		// Sets the authority for this page
		void setAuthority(bool info);
		
		// Ends the usage of the page (primarily for use with authority)
		void endUsage();
	};
}