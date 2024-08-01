#include <iostream>

using std::string;

namespace MaGE {
	namespace Global {
		
	}
	// represents ANSI color sequences
	enum Color {
		RED = 31,
		GREEN = 32,
		YELLOW = 33,
		BLUE = 34,
		MAGENTA = 35,
		CYAN = 36,
		WHITE = 37
	};
	
	// colors <message> with given color
	string 	colorize(string message, Color color);

	// prints <message>
	void 		print(string message, std::ostream &);
}