#pragma once

/* Defines the MaGE Virtual Machine Instructions */

// The MaGE memory system works by having 3 primary memory regions
// 1. @world - The main system page, this is where any information is stored, this data is persistent (see: default.mgVK)
// 2. @user  - The user page, any temporary information should be stored here. Good for temporary data
// 3. @protected - The protected page, any data stored here can only be used by one instance at a time.
//
// MaGE by default loads into the default.mgVK file. This contains the basic version information,
// as well as the default world page.
// 
// A majority of the time, the @world page will have the most information stored.
// 
// By default, the size arrays look like:
// 
//	* @world - 1MB of data
//  * @user - ~0.5MB of data (can be higher, but must not exceed @world)
//	* @protected - infinitely large, can only be accessed by the current instance
//
// default_@user.mgVK is an example of a user page.
// default_@protected.mgVK is an example of a protected page. When accessed, this page will be locked to the current instance.

namespace MaGE {
	enum MaGEInstructionSet {
		HALT = 0x00,		// Halt the program
		EARLY = 0x01,		// Early halt the program
		STORE = 0x02,		// Store a value					[@region] [ADDRESS] [SIZE] [VALUE]
		LOAD = 0x03,		// Load a value w/ size		[@region] [ADDRESS] [SIZE] [ALIGN]
	};
}

