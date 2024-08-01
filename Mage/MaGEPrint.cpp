#include <iostream>
#include <string>

#include "MaGEPrint.h"

void MaGE::print(string message, std::ostream & str) {
	str << "mage: " << message << std::endl;
}

string MaGE::colorize(string message, Color color) {
	return "\u001b[" + std::to_string(color) + "m" + message + "\u001b[0m";
}
