#pragma once

#ifndef MY_WHEEL
#define MY_WHEEL

#include <wheel_core/ram.hpp>
#include <wheel_core/api.hpp>

#define IBUFFER_GAMEPADS_OFFSET 0
#define IBUFFER_MOUSE_OFFSET (IBUFFER_GAMEPADS_OFFSET + GAMEPADS_SIZE)
#define IBUFFER_KEYBOARD_OFFSET (IBUFFER_MOUSE_OFFSET + MOUSE_SIZE)

class GraphicsDevice {
private:
    
public:
    GraphicsDevice(/* args */) = default;
    ~GraphicsDevice() = default;

    std::array<uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT + 1> gbuffer;
    virtual void draw();
};

class AudioDevice {
private:
    
public:
    AudioDevice(/* args */) = default;
    ~AudioDevice() = default;
    std::array<uint8_t, SOUND_REGISTER_SIZE> abuffer;
    virtual void playSound(){}
};

class InputDevice {
private:

public:
    InputDevice() = default;
    ~InputDevice() = default;
    std::array<uint8_t, GAMEPADS_SIZE + MOUSE_SIZE + KEYBOARD_SIZE> ibuffer;
    uint8_t wheelSystemByte = 0;
    // 7 6 5 4 3 2 1 0
    // 0 - power on / off (1 for off)
    // 1-7 - reserved
    virtual void getInput(){}
};


class MyWheel {
public:
	MyWheel(GraphicsDevice* pgd, AudioDevice* pad, InputDevice* pid);
	~MyWheel();
	void display();
	
	int wheelStart();
	TicOrientedRam* ram;
    uint64_t timer = 0;
	std::array<uint8_t, GAMEPADS_SIZE> btnp_flags;
	uint8_t keyp_flags;
private:
	int frame_rate;
	GraphicsDevice* graphicsDevice;
	AudioDevice* audioDevice;
	InputDevice* inputDevice;
	void updateInput();

};

#endif // !MY_WHEEL
