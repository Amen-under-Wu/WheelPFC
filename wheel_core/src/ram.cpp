#include <wheel_core/ram.hpp>

TicOrientedRam::TicOrientedRam(): 
	vbanks(VramBanks()),
	current_vbank(0), 
	ram_after_vram(std::array<uint8_t, RAM_SIZE - VRAM_SIZE>()){
	for (int i = 0; i < PALETTE_MAP_SIZE * 2; ++i)
		vbanks[0][PALETTE_MAP_OFFSET + i / 2] += i << ((i % 2) * 4);
	std::array<uint8_t, PALETTE_SIZE> DB16_palette({
		0x14, 0x0c, 0x1c, //black 0
		0x44, 0x24, 0x34, //dark magenta 1
		0x30, 0x34, 0x6d, //dark blue 2
		0x4e, 0x4a, 0x4e, //dark grey 3
		0x85, 0x4c, 0x30, //dark brown 4
		0x34, 0x65, 0x24, //oliver green 5
		0xd0, 0x46, 0x48, //red 6
		0x75, 0x71, 0x61, //grey 7
		0x59, 0x7d, 0xce, //blue 8
		0xd2, 0x7d, 0x2c, //orange 9
		0x85, 0x95, 0xa1, //light grey 10
		0x6d, 0xaa, 0x2c, //green 11
		0xd2, 0xaa, 0x99, //misty rose 12
		0x6d, 0xc2, 0xca, //sky blue 13
		0xda, 0xd4, 0x5e, //yellow 14
		0xde, 0xee, 0xd6, //white 15
    });
    std::array<uint8_t, PALETTE_SIZE> tic80_default_palette({
        0x1a, 0x1c, 0x2c, //black 0
        0x5d, 0x27, 0x5d, //purple 1
        0xb1, 0x3e, 0x53, //red 2
        0xef, 0x7d, 0x57, //orange 3
        0xff, 0xcd, 0x75, //yellow 4
        0xa7, 0xf0, 0x70, //light green 5
        0x38, 0xb7, 0x64, //green 6
        0x25, 0x71, 0x79, //dark green 7
        0x29, 0x36, 0x6f, //dark blue 8
        0x3b, 0x5d, 0xc9, //blue 9
        0x41, 0xa6, 0xf6, //light blue 10
        0x73, 0xef, 0xf7, //cyan 11
        0xf4, 0xf4, 0xf4, //white 12
        0x94, 0xb0, 0xc2, //light grey 13
        0x56, 0x6c, 0x86, //grey 14
        0x33, 0x3c, 0x57, //dark grey 15
    });
	for (int i = 0; i < PALETTE_SIZE; ++i) 
		vbanks[0][PALETTE_OFFSET + i] = tic80_default_palette[i];
	
	vbanks[1] = vbanks[0];

    uint8_t sys_font[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x1f, 0x0a, 0x1f, 0x0a, 0x00, 0x00, 0x00, 0x1e, 0x05, 0x0e, 0x14, 0x0f, 0x00, 0x00, 0x00, 0x11, 0x08, 0x04, 0x02, 0x11, 0x00, 0x00, 0x00, 0x02, 0x05, 0x16, 0x09, 0x16, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x04, 0x04, 0x08, 0x00, 0x00, 0x00, 0x02, 0x04, 0x04, 0x04, 0x02, 0x00, 0x00, 0x00, 0x04, 0x15, 0x0e, 0x15, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x1b, 0x17, 0x13, 0x0e, 0x00, 0x00, 0x00, 0x0c, 0x0e, 0x0c, 0x0c, 0x1e, 0x00, 0x00, 0x00, 0x0f, 0x18, 0x0e, 0x03, 0x1f, 0x00, 0x00, 0x00, 0x1f, 0x18, 0x0c, 0x19, 0x0e, 0x00, 0x00, 0x00, 0x0c, 0x0e, 0x0b, 0x1f, 0x08, 0x00, 0x00, 0x00, 0x1f, 0x03, 0x0f, 0x18, 0x0f, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x0f, 0x13, 0x0e, 0x00, 0x00, 0x00, 0x1f, 0x18, 0x0c, 0x06, 0x03, 0x00, 0x00, 0x00, 0x0e, 0x13, 0x0e, 0x13, 0x0e, 0x00, 0x00, 0x00, 0x0e, 0x13, 0x1e, 0x10, 0x0e, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x06, 0x04, 0x02, 0x00, 0x00, 0x08, 0x04, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x1e, 0x18, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0e, 0x15, 0x1d, 0x01, 0x0e, 0x00, 0x00, 0x00, 0x0e, 0x13, 0x13, 0x1f, 0x13, 0x00, 0x00, 0x00, 0x0f, 0x13, 0x0f, 0x13, 0x0f, 0x00, 0x00, 0x00, 0x0e, 0x13, 0x03, 0x13, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x13, 0x13, 0x13, 0x0f, 0x00, 0x00, 0x00, 0x1f, 0x03, 0x0f, 0x03, 0x1f, 0x00, 0x00, 0x00, 0x1f, 0x03, 0x0f, 0x03, 0x03, 0x00, 0x00, 0x00, 0x1e, 0x03, 0x1b, 0x13, 0x1e, 0x00, 0x00, 0x00, 0x13, 0x13, 0x1f, 0x13, 0x13, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0x00, 0x00, 0x1f, 0x18, 0x18, 0x1b, 0x0e, 0x00, 0x00, 0x00, 0x13, 0x0b, 0x07, 0x0b, 0x13, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x1f, 0x00, 0x00, 0x00, 0x1b, 0x1f, 0x1f, 0x15, 0x11, 0x00, 0x00, 0x00, 0x13, 0x17, 0x1f, 0x1b, 0x13, 0x00, 0x00, 0x00, 0x0e, 0x13, 0x13, 0x13, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x13, 0x13, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x0e, 0x13, 0x13, 0x13, 0x0e, 0x10, 0x00, 0x00, 0x0f, 0x13, 0x13, 0x0f, 0x13, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x0e, 0x1c, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x13, 0x13, 0x13, 0x13, 0x0e, 0x00, 0x00, 0x00, 0x13, 0x13, 0x13, 0x0e, 0x04, 0x00, 0x00, 0x00, 0x11, 0x15, 0x1f, 0x1f, 0x1b, 0x00, 0x00, 0x00, 0x13, 0x13, 0x0e, 0x13, 0x13, 0x00, 0x00, 0x00, 0x16, 0x16, 0x1e, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x1f, 0x0c, 0x06, 0x03, 0x1f, 0x00, 0x00, 0x00, 0x0c, 0x04, 0x04, 0x04, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x06, 0x04, 0x04, 0x04, 0x06, 0x00, 0x00, 0x00, 0x04, 0x0a, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x19, 0x19, 0x1e, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x13, 0x13, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x07, 0x1e, 0x00, 0x00, 0x00, 0x18, 0x1e, 0x19, 0x19, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1b, 0x07, 0x0e, 0x00, 0x00, 0x00, 0x1c, 0x06, 0x1f, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x19, 0x1f, 0x18, 0x0e, 0x00, 0x00, 0x03, 0x0f, 0x13, 0x13, 0x13, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x18, 0x19, 0x0e, 0x00, 0x00, 0x03, 0x13, 0x0f, 0x13, 0x13, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x1f, 0x15, 0x15, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x13, 0x13, 0x13, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x13, 0x13, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x13, 0x13, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x1e, 0x19, 0x19, 0x1e, 0x18, 0x00, 0x00, 0x00, 0x0f, 0x13, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x1c, 0x0f, 0x00, 0x00, 0x00, 0x06, 0x1f, 0x06, 0x06, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x13, 0x13, 0x13, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x13, 0x13, 0x0e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x11, 0x15, 0x1f, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x0e, 0x0e, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x19, 0x19, 0x1e, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x1f, 0x0c, 0x06, 0x1f, 0x00, 0x00, 0x00, 0x0c, 0x04, 0x06, 0x04, 0x0c, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x06, 0x04, 0x0c, 0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0x14, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00,};
    for (int i = 0; i < FONT_PARAM_OFFSET_RELATIVE; ++i) {
        (*this)[SYSTEM_FONT_OFFSET + i] = sys_font[i];
    }
    uint8_t alt_font[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x07, 0x05, 0x07, 0x05, 0x00, 0x00, 0x00, 0x06, 0x03, 0x06, 0x03, 0x02, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02, 0x01, 0x04, 0x00, 0x00, 0x00, 0x03, 0x03, 0x07, 0x05, 0x06, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x02, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x06, 0x05, 0x05, 0x05, 0x03, 0x00, 0x00, 0x00, 0x02, 0x03, 0x02, 0x02, 0x07, 0x00, 0x00, 0x00, 0x03, 0x04, 0x02, 0x01, 0x07, 0x00, 0x00, 0x00, 0x03, 0x04, 0x02, 0x04, 0x03, 0x00, 0x00, 0x00, 0x05, 0x05, 0x07, 0x04, 0x04, 0x00, 0x00, 0x00, 0x07, 0x01, 0x03, 0x04, 0x03, 0x00, 0x00, 0x00, 0x06, 0x01, 0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x07, 0x04, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x07, 0x05, 0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x07, 0x05, 0x07, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x07, 0x04, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x05, 0x07, 0x01, 0x06, 0x00, 0x00, 0x00, 0x02, 0x05, 0x07, 0x05, 0x05, 0x00, 0x00, 0x00, 0x03, 0x05, 0x03, 0x05, 0x03, 0x00, 0x00, 0x00, 0x06, 0x01, 0x01, 0x01, 0x06, 0x00, 0x00, 0x00, 0x03, 0x05, 0x05, 0x05, 0x03, 0x00, 0x00, 0x00, 0x07, 0x01, 0x03, 0x01, 0x07, 0x00, 0x00, 0x00, 0x07, 0x01, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x05, 0x05, 0x06, 0x00, 0x00, 0x00, 0x05, 0x05, 0x07, 0x05, 0x05, 0x00, 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00, 0x00, 0x07, 0x04, 0x04, 0x05, 0x02, 0x00, 0x00, 0x00, 0x05, 0x05, 0x03, 0x05, 0x05, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x07, 0x00, 0x00, 0x00, 0x07, 0x07, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x03, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x02, 0x05, 0x05, 0x05, 0x02, 0x00, 0x00, 0x00, 0x03, 0x05, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02, 0x05, 0x05, 0x07, 0x06, 0x00, 0x00, 0x00, 0x03, 0x05, 0x07, 0x03, 0x05, 0x00, 0x00, 0x00, 0x06, 0x01, 0x02, 0x04, 0x03, 0x00, 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x05, 0x06, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x05, 0x02, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x07, 0x07, 0x00, 0x00, 0x00, 0x05, 0x05, 0x02, 0x05, 0x05, 0x00, 0x00, 0x00, 0x05, 0x05, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x07, 0x04, 0x02, 0x01, 0x07, 0x00, 0x00, 0x00, 0x06, 0x02, 0x02, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x05, 0x07, 0x00, 0x00, 0x00, 0x01, 0x03, 0x05, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x01, 0x06, 0x00, 0x00, 0x00, 0x04, 0x06, 0x05, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x05, 0x03, 0x06, 0x00, 0x00, 0x00, 0x04, 0x02, 0x07, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x06, 0x05, 0x07, 0x04, 0x02, 0x00, 0x00, 0x01, 0x03, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x05, 0x02, 0x00, 0x00, 0x01, 0x05, 0x03, 0x03, 0x05, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x03, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x02, 0x05, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x05, 0x05, 0x03, 0x01, 0x00, 0x00, 0x00, 0x06, 0x05, 0x05, 0x06, 0x04, 0x00, 0x00, 0x00, 0x05, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x04, 0x03, 0x00, 0x00, 0x00, 0x02, 0x07, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x05, 0x02, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x06, 0x04, 0x02, 0x00, 0x00, 0x00, 0x07, 0x06, 0x01, 0x07, 0x00, 0x00, 0x00, 0x06, 0x02, 0x03, 0x02, 0x06, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x03, 0x02, 0x06, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, };
    for (int i = 0; i < FONT_PARAM_OFFSET_RELATIVE; ++i) {
        (*this)[ALT_FONT_OFFSET + i] = alt_font[i];
    }
    (*this)[SYSTEM_FONT_OFFSET + FONT_PARAM_OFFSET_RELATIVE] = 0x06;
    (*this)[SYSTEM_FONT_OFFSET + FONT_PARAM_OFFSET_RELATIVE + 1] = 0x06;
    (*this)[ALT_FONT_OFFSET + FONT_PARAM_OFFSET_RELATIVE] = 0x04;
    (*this)[ALT_FONT_OFFSET + FONT_PARAM_OFFSET_RELATIVE + 1] = 0x06;

    uint8_t default_gamepad_mapping[GAMEPAD_MAPPING_SIZE] = {58,59,60,61,26,24,1,19,};
    for (int i = 0; i < GAMEPAD_MAPPING_SIZE; ++i) {
        (*this)[GAMEPAD_MAPPING_OFFSET + i] = default_gamepad_mapping[i];
    }
}

TicOrientedRam::~TicOrientedRam(){};

uint8_t& TicOrientedRam::operator[](size_t i) {
	if (i < VRAM_SIZE)
		return vbanks[current_vbank][i];
	else
		return ram_after_vram[i - VRAM_SIZE];
}

void TicOrientedRam::switchVbank(int i) {
	if (i >= 0 && i < VBANK_SIZE)
        current_vbank = i;
}

const VramBanks& TicOrientedRam::vbref() const{
	return vbanks;
}
