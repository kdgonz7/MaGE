#pragma once

/* we need to add 4, since we are starting at 0. 12 will override the last number of the header.
	n'd everything gets all screwed up. */
constexpr auto MAGE_HEADERSIZE = 12 + 4;

// mage header parts
constexpr auto MAGE_HEADER_PART_1 = 0xFF;
constexpr auto MAGE_HEADER_PART_1_LOCKED = 0xFE; // if the file is authority-locked, then this should be different.
constexpr auto MAGE_HEADER_PART_2 = 0xAF;
constexpr auto MAGE_HEADER_PART_3 = 0xBF;
constexpr auto MAGE_HEADER_VERSION = 0001;
constexpr auto MAGE_DEFAULT_UNINITIALIZED = -1;

constexpr auto EMEMORYOVERFLOW = 2;


