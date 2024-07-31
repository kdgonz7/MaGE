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

		const char* nameOf() { return page_name;  }
		const size_t sizeOf() { return max_size;  }
		const char* getFilename() { return filename; }
		const int32_t getVersion() { return version; }

		/// <summary>
		/// Allocates a new page with a size of `this.max_size`
		/// </summary>
		/// <returns>the new raw page</returns>
		void* allocate();

		/// <summary>
		/// Writes to the data at the offset address. This will overwrite any existing data.
		/// </summary>
		/// <param name="data"></param>
		/// <param name="size"></param>
		/// <param name="offset"></param>
		void write(const char* data, const size_t size, const size_t offset = 0, bool skip_header = true);

		/// <summary>
		/// Writes to the page where the data is 0
		/// </summary>
		/// <param name="info"></param>
		/// <param name="size"></param>
		void writeWhereFree(const char* info, const size_t size);

		/// <summary>
		/// Creates a new page file
		/// </summary>
		/// <param name="filename"></param>
		void dumpToFile(const char* filename);

		/// <summary>
		/// Loads this page from the specified file
		/// </summary>
		void load(const char* filename);

		/// <summary>
		/// Reads an integer from the specified offset
		/// </summary>
		/// <param name="offset">The offset to read from</param>
		/// <param name="endianness">The endianness of the integer</param>
		/// <returns></returns>
		int32_t readInt32(const size_t offset = 0, MaGE::Endian endianness = MaGE::Endian::ENDIAN_LITTLE, bool skip_header = true);
		int64_t readInt64(const size_t offset = 0, MaGE::Endian endianness = MaGE::Endian::ENDIAN_LITTLE) const;

		/// <summary>
		/// Writes an integer to the specified offset
		/// </summary>
		/// <param name="offset">The offset to write to</param>
		/// <param name="value">The value to write</param>
		/// <param name="endianness">The endianness of the integer</param>
		/// <returns></returns>
		void writeInt32(const size_t offset, const int32_t value, MaGE::Endian endianness = MaGE::Endian::ENDIAN_LITTLE, bool skip_header = true);
		void writeInt64(const size_t offset, const int64_t value, MaGE::Endian endianness = MaGE::Endian::ENDIAN_LITTLE);

		/// <summary>
		/// Checks if the data stream has the magic number
		/// </summary>
		/// <param name="data"></param>
		/// <param name="size"></param>
		/// <returns></returns>
		bool dataStreamHasMagic(const char* data);

		/// <summary>
		/// Essentially `malloc(size)`
		/// </summary>
		/// <param name="size"></param>
		/// <returns></returns>
		MaGE::GE_MemoryObject *ask(size_t size);

		/// <summary>
		/// Sets the authority for this page
		/// </summary>
		/// <param name="info"></param>
		/// <param name="size"></param>
		void setAuthority(bool info);
		
		/// <summary>
		/// Ends the usage of the page (primarily for use with authority)
		/// </summary>
		void endUsage();
	};
}