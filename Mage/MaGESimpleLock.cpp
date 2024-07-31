#include <fstream>
#include <iostream>
#include <cstdio>

#include "MaGESimpleLock.h"

bool MaGE::simpleLock(const char* filename)
{
	// create a file that is .<filename>.lock in the same directory,
	// and if it exists, then it is already locked,
	// and we return false

	std::string lockfilename = std::string(filename) + ".lock";

	struct stat buffer;
	if (stat(lockfilename.c_str(), &buffer) == 0) {
		return false;
	}

	std::ofstream lock_file(lockfilename);

	lock_file.write("// created by MaGE", 20);
	lock_file.close();

	return true;
}

bool MaGE::unLock(const char* filename)
{
	// delete the .<filename>.lock file
	std::remove((std::string(filename) + ".lock").c_str());
	
	struct stat buffer;

	int res = stat(filename, &buffer);
	if (res != 0) {
		return false;
	}

	// if it still exists, lock was unsuccessful
	return res == 0;
}
