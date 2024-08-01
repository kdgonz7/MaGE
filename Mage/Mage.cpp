// Mage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "MaGEPrint.h"
#include "MaGEPage.h"

static int stage1()
{
	std::cout << "stage1 - creating the initial @world page\n";

	MaGE::GE_Page page = MaGE::GE_Page("default_@world.mgVK", 1024);
	
	page.setAuthority(false); // if you use authority, you MUST call endUsage() or it will lock the file and you won't be able to load it
	page.allocate(); // call allocate if you're making a new page, otherwise you can just load it
	
	page.writeInt32(0, 15, MaGE::ENDIAN_LITTLE); // write to the page
	page.dumpToFile("default_@world.mgVK"); // dump the page to a file

	std::cout << "---------" << std::endl;

	return 0;
}

static int stage2()
{
	std::cout << "stage2 - reading from the @world page once, simple integer\n";

	using MaGE::GE_Page;

	GE_Page page = GE_Page("@world", -1); // -1 = loaded file, size will be appointed

	page.load("default_@world.mgVK"); // load the buffer from the .mgVK file

	int n = page.readInt32(0, MaGE::ENDIAN_LITTLE); // read a little integer to test

	std::cout << "mage version is: " << page.getVersion() << std::endl;
	std::cout << "found read integer at address 0x00: " << n << std::endl;
	
	std::cout << "---------" << std::endl;

	return 0;
}

static int stage3()
{
	// MaGE, aside from classes, uses little
	// to no heap memory, it uses a simple file 
	// format that is essentially raw memory.
	std::cout << "stage3 - reading from the @world page and returning a memory pointer of 3 bytes\n";

	using MaGE::GE_Page;

	// testing memory allocation
	GE_Page pagefile = GE_Page("@world", -1);
	pagefile.load("default_@world.mgVK");

	// get a pointer to a free memory address (an address with 0x00)
	auto m = pagefile.ask(4);

	if (m == nullptr or ! m->isGood()) {
		std::cout << "stage3: failed to allocate memory" << std::endl;
	}

	char * p = m->getMemory();
	p[0] = 'a';
	p[1] = 'b';
	p[2] = 'c';
	p[3] = 0;

	// keep in mind, when this object is destroyed, the memory will be gone FOREVER,
	// to store memory, you need to write it to the file you're reading from., or allocate, and dump it to a file.
	std::cout << "allocated a free block of memory at: " << m->getOffset() << std::endl;
	std::cout << reinterpret_cast<char*>(m->getMemory()) << std::endl;

	std::cout << "---------" << std::endl;

	delete m; // you have to free memory objects alloctaed with ask()

	// note that you usually don't want to write 
	// temporary data to the @world page, since that 
	// gets saved to the file, and the data will remain there forever.

	pagefile.dumpToFile("default_@world_dirty.mgVK");

	return 0;
}

int stage4(void)
{
	std::cout << "stage4 - creating a @protected page and reading from it twice (error)\n";

	using MaGE::GE_Page;

	GE_Page protectedfile = GE_Page("@protected", 1024 ^ 2);
	protectedfile.setAuthority(true);
	protectedfile.allocate();

	protectedfile.dumpToFile("default_@protected.mgVK");

	GE_Page pagefile = GE_Page("@protected", -1);
	pagefile.setAuthority(true);
	pagefile.load("default_@protected.mgVK");

	// this is an error, because we're not authorized, only one instance can read
	//GE_Page pagefile2_error = GE_Page("@protected", -1);
	//pagefile.setAuthority(true);
	//pagefile2_error.load("default_@protected.mgVK"); // error, because we're not authorized, only one instance can read

	pagefile.endUsage();
	std::cout << "---------" << std::endl;

	return 0;
}

int main(void)
{
	string ms1 = MaGE::colorize("test", MaGE::Color::RED);
	MaGE::print(ms1, std::cout);

	stage3();

	exit(0);
}

