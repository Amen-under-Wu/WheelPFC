#include <wheel_core/my_wheel.hpp>
#include <wheel_core/api.hpp>
#include <iostream>
#include <chrono>

void GraphicsDevice::draw() {}

MyWheel::MyWheel(GraphicsDevice* pgd, AudioDevice* pad, InputDevice* pid) :
	ram(new TicOrientedRam),
	frame_rate(60), 
	graphicsDevice(pgd),
	audioDevice(pad),
	inputDevice(pid){}

MyWheel::~MyWheel() {
	
}

inline void MyWheel::updateInput() {
    for (int i = 0; i < KEYBOARD_SIZE; ++i) {
        for (int j = 0; j < GAMEPAD_MAPPING_SIZE; ++j) {
            if ((*ram)[KEYBOARD_OFFSET + i] == (*ram)[GAMEPAD_MAPPING_OFFSET + j]) {
                inputDevice->ibuffer[IBUFFER_GAMEPADS_OFFSET + j / 8] |= 1u << (j % 8);
            }
        }
    }
    for (int i = 0; i < GAMEPADS_SIZE; ++i) {
        btnp_flags[i] = ((*ram)[GAMEPADS_OFFSET + i] ^ inputDevice->ibuffer[IBUFFER_GAMEPADS_OFFSET + i]) & inputDevice->ibuffer[IBUFFER_GAMEPADS_OFFSET + i];
        (*ram)[GAMEPADS_OFFSET + i] = inputDevice->ibuffer[IBUFFER_GAMEPADS_OFFSET + i];
    }

	//key bit number i: (1 << i)
	for (int i = 0; i < KEYBOARD_SIZE; ++i) { //can be improved
		for (int j = 0; j < KEYBOARD_SIZE; ++j) {
			if (inputDevice->ibuffer[IBUFFER_KEYBOARD_OFFSET + i] == (*ram)[KEYBOARD_OFFSET + j]) {
				keyp_flags &= 0xf ^ (1u << i);
				break;
			}
			keyp_flags |= 1u << i;
		}
	}
	for (int i = 0; i < KEYBOARD_SIZE; ++i) {
		(*ram)[KEYBOARD_OFFSET + i] = inputDevice->ibuffer[IBUFFER_KEYBOARD_OFFSET + i];
	}

    for (int i = 0; i < MOUSE_SIZE; ++i) {
        (*ram)[MOUSE_OFFSET + i] = inputDevice->ibuffer[IBUFFER_MOUSE_OFFSET + i];
    }
}

int MyWheel::wheelStart() {
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double, std::milli>> sim_time(std::chrono::high_resolution_clock::now());
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double, std::milli>> real_time(std::chrono::high_resolution_clock::now());
	std::chrono::duration<double, std::milli> fixed_duration((1000.0 / frame_rate));
	try {
        timer = 0;
		init();
		while ((inputDevice->wheelSystemByte & 0x1) == 0) {
			real_time = std::chrono::high_resolution_clock::now();
			while (sim_time < real_time) {
				inputDevice->getInput();
				//get other events here
				updateInput();
				update();
				display();
                timer += 1;
				sim_time += fixed_duration;
                if (real_time - sim_time > fixed_duration) {
                    sim_time = real_time;
                }
			}
		}
		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}


#define GET_COLOR_ID(color_id,vbank_id) ((ram->vbref()[vbank_id][PALETTE_MAP_OFFSET + (color_id) / 2] >> (((color_id) % 2) * 4)) & 0x0f)
#define GET_COLOR(color_id,vbank_id) ((vbanks[vbank_id][PALETTE_OFFSET + (color_id) * 3] << 16) + (vbanks[vbank_id][PALETTE_OFFSET + (color_id) * 3 + 1] << 8) + (vbanks[vbank_id][PALETTE_OFFSET + (color_id) * 3 + 2]))
void MyWheel::display() {
	const VramBanks& vbanks = ram->vbref();
	uint8_t border_color_id = GET_COLOR_ID(vbanks[0][BORDER_COLOR_OFFSET], 0);
	uint32_t color;
	for (int i = 0; i < SCREEN_HEIGHT; ++i) {
		scanline(i);
		for (int j = 0; j < SCREEN_WIDTH; ++j) {
			uint8_t color_id = GET_COLOR_ID((vbanks[1][(i * SCREEN_WIDTH + j) / 2] >> ((j % 2) * 4)) & 0xf, 1);
			if (color_id == vbanks[1][BORDER_COLOR_OFFSET])
				color = GET_COLOR(GET_COLOR_ID(((vbanks[0][(i * SCREEN_WIDTH + j) / 2] >> ((j % 2) * 4))) & 0xf, 0), 0);
			else
				color = GET_COLOR(color_id, 1);
			graphicsDevice->gbuffer[i * SCREEN_WIDTH + j] = color;
		}

	}
	graphicsDevice->gbuffer[SCREEN_WIDTH * SCREEN_HEIGHT] = GET_COLOR(border_color_id, 0);
	graphicsDevice->draw();
}

#undef GET_COLOR_ID
#undef GET_COLOR