/*
+-----------------------------------+
|           96KB RAM LAYOUT         |
+-------+-------------------+-------+
| ADDR  | INFO              | BYTES |
+-------+-------------------+-------+
| 00000 | <VRAM>            | 16384 |
| 04000 | TILES             | 8192  |
| 06000 | SPRITES           | 8192  |
| 08000 | MAP               | 32640 |
| 0FF80 | GAMEPADS          | 4     |
| 0FF84 | MOUSE             | 4     |
| 0FF88 | KEYBOARD          | 4     |
| 0FF8C | SFX STATE         | 16    |
| 0FF9C | SOUND REGISTERS   | 72    |
| 0FFE4 | WAVEFORMS         | 256   |
| 100E4 | SFX               | 4224  |
| 11164 | MUSIC PATTERNS    | 11520 |
| 13E64 | MUSIC TRACKS      | 408   |
| 13FFC | MUSIC STATE       | 4     |
| 14000 | STEREO VOLUME     | 4     |
| 14004 | PERSISTENT MEMORY | 1024  |
| 14404 | SPRITE FLAGS      | 512   |
| 14604 | FONT              | 1016  |
| 149FC | FONT PARAMS       | 8     |
| 14A04 | ALT FONT          | 1016  |
| 14DFC | ALT FONT PARAMS   | 8     |
| 14E04 | BUTTONS MAPPING   | 32    |
| 14E24 | ** RESERVED **    | 12764 |
+-------+-------------------+-------+


+-----------------------------------+
|          16KB VRAM LAYOUT         |
+-------+-------------------+-------+
| ADDR  | INFO              | BYTES |
+-------+-------------------+-------+
| 00000 | SCREEN            | 16320 |
| 03FC0 | PALETTE           | 48    |
| 03FF0 | PALETTE MAP       | 8     |
| 03FF8 | BORDER COLOR      | 1     | or OVR TRANSPARENT COLOR
| 03FF9 | SCREEN OFFSET     | 2     |
| 03FFB | MOUSE CURSOR      | 1     |
| 03FFC | BLIT SEGMENT      | 1     |
| 03FFD | ... (reserved)    | 3     |
+-------+-------------------+-------+*/




#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

#ifndef MY_RAM
#define MY_RAM

#define RAM_SIZE 0x18000
#define VRAM_SIZE 0x4000

#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_DATA_SIZE (TILE_WIDTH * TILE_HEIGHT / 2)
#define TILE_CANVAS_WIDTH 16
#define TILE_CANVAS_HEIGHT 16
#define TILE_CANVAS_SIZE (TILE_CANVAS_WIDTH * TILE_CANVAS_HEIGHT)
#define SPRITE_WIDTH 8
#define SPRITE_HEIGHT 8
#define SPRITE_DATA_SIZE (SPRITE_WIDTH * SPRITE_HEIGHT / 2)
#define SPRITE_CANVAS_WIDTH 16
#define SPRITE_CANVAS_HEIGHT 16
#define SPRITE_CANVAS_SIZE (SPRITE_CANVAS_WIDTH * SPRITE_CANVAS_HEIGHT)
#define MAP_WIDTH 240
#define MAP_HEIGHT 136
#define MAP_DATA_SIZE 1

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 136

#define PALETTE_OFFSET (SCREEN_WIDTH * SCREEN_HEIGHT / 2)
#define COLOR_SIZE 3
#define PALETTE_SIZE (COLOR_SIZE * 16)

#define PALETTE_MAP_OFFSET (PALETTE_OFFSET + PALETTE_SIZE)
#define PALETTE_MAP_SIZE 8

#define BORDER_COLOR_OFFSET (PALETTE_MAP_OFFSET + PALETTE_MAP_SIZE)
#define BORDER_COLOR_SIZE 1


#define TILES_OFFSET (VRAM_SIZE)
#define TILES_SIZE (TILE_DATA_SIZE * TILE_CANVAS_SIZE)

#define SPRITES_OFFSET ((TILES_OFFSET) + (TILES_SIZE))
#define SPRITES_SIZE ((SPRITE_DATA_SIZE) * (SPRITE_CANVAS_SIZE))

#define MAP_OFFSET ((SPRITES_OFFSET) + (SPRITES_SIZE))
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT * MAP_DATA_SIZE)

#define GAMEPADS_OFFSET (MAP_OFFSET + MAP_SIZE)
#define GAMEPADS_SIZE 4

#define MOUSE_OFFSET (GAMEPADS_OFFSET + GAMEPADS_SIZE)
#define MOUSE_SIZE 4

#define KEYBOARD_OFFSET (MOUSE_OFFSET + MOUSE_SIZE)
#define KEYBOARD_SIZE 4

#define SOUND_REGISTER_OFFSET (KEYBOARD_OFFSET + KEYBOARD_SIZE)
#define SOUND_REGISTER_DATA_SIZE 18
#define CHANNEL_AMOUNT 4
#define SOUND_REGISTER_SIZE (SOUND_REGISTER_DATA_SIZE * CHANNEL_AMOUNT)
#define WAVEFORMS_OFFSET (SOUND_REGISTER_OFFSET + SOUND_REGISTER_SIZE)
#define WAVEFORMS_SIZE 256 //need update from this part on
#define SFX_OFFSET (WAVEFORMS_OFFSET + WAVEFORMS_SIZE)
#define SFX_SIZE 4224

#define MUSIC_PATTERNS_OFFSET (SFX_OFFSET + SFX_SIZE)
#define MUSIC_PATTERNS_SIZE 11520
#define MUSIC_TRACKS_OFFSET (MUSIC_PATTERNS_OFFSET + MUSIC_PATTERNS_SIZE)
#define MUSIC_TRACKS_SIZE 408
#define MUSIC_STATE_OFFSET (MUSIC_TRACKS_OFFSET + MUSIC_TRACKS_SIZE)
#define MUSIC_STATE_SIZE 4

#define STEREO_VOLUME_OFFSET (MUSIC_STATE_OFFSET + MUSIC_STATE_SIZE)
#define STEREO_VOLUME_SIZE 4

#define PMEM_OFFSET (STEREO_VOLUME_OFFSET + STEREO_VOLUME_SIZE)
#define PMEM_SIZE 1024

#define SPRITE_FLAG_OFFSET (PMEM_OFFSET + PMEM_SIZE)
#define SPRITE_FLAG_SIZE (TILE_CANVAS_SIZE + SPRITE_CANVAS_SIZE)

#define SYSTEM_FONT_OFFSET (SPRITE_FLAG_OFFSET + SPRITE_FLAG_SIZE)
#define FONT_SIZE_PER_CHAR 8
#define FONT_SIZE (128 * FONT_SIZE_PER_CHAR)
#define FONT_PARAM_OFFSET_RELATIVE (FONT_SIZE - 8)

#define ALT_FONT_OFFSET (SYSTEM_FONT_OFFSET + FONT_SIZE)

#define GAMEPAD_MAPPING_OFFSET (ALT_FONT_OFFSET + FONT_SIZE)
#define GAMEPAD_MAPPING_SIZE 32

#define VBANK_SIZE 2
#define BANK_SIZE 8

typedef std::array<uint8_t, VRAM_SIZE> VramBank;
typedef std::array<VramBank, VBANK_SIZE> VramBanks;

class TicOrientedRam {
public:
	TicOrientedRam ();
	~TicOrientedRam ();
	uint8_t& operator[](size_t i);
	void switchVbank(int i);
	const VramBanks& vbref() const;
private:
	VramBanks vbanks;
	uint8_t current_vbank;
	std::array<uint8_t, RAM_SIZE - VRAM_SIZE> ram_after_vram;
};

#endif //MY_RAM
