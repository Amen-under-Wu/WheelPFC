#include <wheel_core/api.hpp>
#include <wheel_core/my_wheel.hpp>
#include <cmath>
#include <ctime>
//#include <algorithm>

extern MyWheel* pWheel;

void poke(uint32_t addr, uint8_t val) {
	pWheel->ram->operator[](addr) = val;
}
void poke4(uint32_t addr_4, uint8_t val) {
	val = val & 0xf;
    pWheel->ram->operator[](addr_4 / 2) &= 0xf << ((!(addr_4 % 2)) * 4);
    pWheel->ram->operator[](addr_4 / 2) |= val << ((addr_4 % 2) * 4);
}

uint8_t peek(uint32_t addr) {
	return pWheel->ram->operator[](addr);
}
uint8_t peek4(uint32_t addr_4) {
	return (pWheel->ram->operator[](addr_4 / 2) >> ((addr_4 % 2) * 4)) & 0xf;
}

void memcpy(uint32_t addr_to, uint32_t addr_from, uint32_t length) {
    for (int i = 0; i < length; ++i) {
        poke(addr_to + i, peek(addr_from + i));
    }
}

void memset(uint32_t addr, uint8_t val, uint32_t length) {
    for (int i = 0; i < length; ++i) {
        poke(addr + i, val);
    }
}

uint32_t pmem(int index) {
    if (index < 0 || index >= PMEM_SIZE / 4)
        return 0;
    uint32_t val = 0;
    for (int i = 0; i < 4; ++i) {
        val |= peek(PMEM_OFFSET + index * 4 + i) << (i * 8);
    }
    return val;
}
void pmem(int index, uint32_t val) {
    if (index < 0 || index >= PMEM_SIZE / 4)
        return;
    for (int i = 0; i < 4; ++i) {
        poke(PMEM_OFFSET + index * 4 + i, val >> (i * 8));
    }
}
void vbank(int id) {
    pWheel->ram->switchVbank(id);
}


void pix(int x, int y, int color_id) {
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
	if (color_id >= 0 && color_id < 16) {
		poke4(y * SCREEN_WIDTH + x, color_id);
	}
}

void pix_unsafe(int x, int y, int color_id) {
    poke4(y * SCREEN_WIDTH + x, color_id);
}

uint8_t pix(int x, int y) {
	return peek4(y * SCREEN_WIDTH + x);
}

void vline(int x, int y1, int y2, int color_id) { //todo: improve
	if (color_id >= 0 && color_id < 16 && x >= 0 && x < SCREEN_WIDTH) {
		if (y1 > y2) {
			vline(x, y2, y1, color_id);
			return;
		}
		if (y1 < 0) y1 = 0;
		if (y2 >= SCREEN_HEIGHT) y2 = SCREEN_HEIGHT - 1;
		for (int y = y1; y <= y2; ++y) {
			pix(x, y, color_id);
		}
	}
}
void hline(int x1, int x2, int y, int color_id) {
	if (color_id >= 0 && color_id < 16 && y >= 0 && y < SCREEN_HEIGHT) {
		if (x1 > x2) {
			hline(x2, x1, y, color_id);
			return;
		}
		if (x1 < 0) x1 = 0;
		if (x2 >= SCREEN_WIDTH) x2 = SCREEN_WIDTH - 1;
		for (int x = x1; x <= x2; ++x) {
			pix(x, y, color_id);
		}
	}
}
void rect(int x1, int y1, int x2, int y2, int color_id) {
	if (color_id >= 0 && color_id < 16) {
		if (x1 == x2 && y1 == y2) {
            pix(x1, y1, color_id);
            return;
        }
		if (y1 > y2) {
			y1 ^= y2;
            y2 ^= y1;
            y1 ^= y2;
		}
		if (y1 < 0) y1 = 0;
		if (y2 >= SCREEN_HEIGHT) y2 = SCREEN_HEIGHT - 1;
		for (int y = y1; y <= y2; ++y) {
			hline(x1, x2, y, color_id);
		}
	}
}
void rectb(int x1, int y1, int x2, int y2, int color_id) {
	hline(x1, x2, y1, color_id);
	hline(x1, x2, y2, color_id);
	vline(x1, y1, y2, color_id);
	vline(x2, y1, y2, color_id);
}

void line(double x1, double y1, double x2, double y2, int color_id) { //can be improved with bresenham algorithm
    if (x1 > x2) {
        x1 += x2; x2 = x1 - x2; x1 -= x2;
        y1 += y2; y2 = y1 - y2; y1 -= y2;
    }
    double dx = x2 - x1;
    double dy = y2 - y1;
    int xi = floor(x1);
    int yi;
    if (dy < 0) {
        yi = ceil(y1);
        if (-dy > dx) {
            while (yi >= y2) {
                pix (xi, yi, color_id);
                yi -= 1;
                if (-(dx * yi - dy * xi - dx * y1 + dy * x1) > dx * yi - dy * (xi + 1) - dx * y1 + dy * x1)
                    xi += 1;
            }
        }
        else {
            while (xi <= x2) {
                pix (xi, yi, color_id);
                xi += 1;
                if (dx * yi - dy * xi - dx * y1 + dy * x1 > -(dx * (yi - 1) - dy * xi - dx * y1 + dy * x1))
                    yi -= 1;
            }
        }

    }
    else {
        yi = floor(y1);
        if (dy > dx) {
            while (yi <= y2) {
                pix (xi, yi, color_id);
                yi += 1;
                if (dx * yi - dy * xi - dx * y1 + dy * x1 > -(dx * yi - dy * (xi + 1) - dx * y1 + dy * x1))
                    xi += 1;
            }
        }
        else {
            while (xi <= x2) {
                pix (xi, yi, color_id);
                xi += 1;
                if (-(dx * yi - dy * xi - dx * y1 + dy * x1) > dx * (yi + 1) - dy * xi - dx * y1 + dy * x1)
                    yi += 1;
            }
        }
    }

}

void circ(int x, int y, int r, int color_id) {
    if (r <= 0 || color_id > 15 || color_id < 0) {
        return;
    }
    hline(x - r, x + r, y, color_id);
    vline(x, y - r, y + r, color_id);
    int rx = r;
    int ry = 0;
    while (rx > ry + 1) {
        ry += 1;
        if (rx * rx + ry * ry - r * r > -((rx - 1) * (rx - 1) + ry * ry - r * r)) {
            rx -= 1;
        }
        hline(x - rx, x + rx, y + ry, color_id);
        hline(x - rx, x + rx, y - ry, color_id);
        hline(x + ry, x - ry, y + rx, color_id);
        hline(x + ry, x - ry, y - rx, color_id);
    }
}
void circb(int x, int y, int r, int color_id) { // can be improved
    if (r <= 0 || color_id > 15 || color_id < 0) {
        return;
    }
    pix(x + r, y, color_id);
    pix(x - r, y, color_id);
    pix(x, y + r, color_id);
    pix(x, y - r, color_id);
    int rx = r;
    int ry = 0;
    while (rx > ry + 1) {
        ry += 1;
        if (rx * rx + ry * ry - r * r > -((rx - 1) * (rx - 1) + ry * ry - r * r)) {
            rx -= 1;
        }
        pix(x + rx, y + ry, color_id);
        pix(x + rx, y - ry, color_id);
        pix(x - rx, y + ry, color_id);
        pix(x - rx, y - ry, color_id);
        pix(x + ry, y + rx, color_id);
        pix(x + ry, y - rx, color_id);
        pix(x - ry, y + rx, color_id);
        pix(x - ry, y - rx, color_id);

    }
}

enum wheel_spr_flip {no_flip = 0, h_flip, v_flip, hv_flip};
enum wheel_spr_rotate_clockwise {deg_0 = 0, deg_90, deg_180, deg_270};
//rotate first

inline void sized_pix(int x, int y, int size, int color_id) {
    if (size > 0)
        rect(x, y, x + size - 1, y + size - 1, color_id);
}

void cls(int color) {
    uint8_t color_id = color;
    color_id %= 16;
    uint8_t mem_val = (color_id << 4) | color_id;
    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH / 2; ++i) {
        pWheel->ram->operator[](i) = mem_val;
    }
}

int crude_log2(uint8_t val) {
    if (val >> 4)
        if (val >> 6)
            if (val >> 7)
                return 7;
            else
                return 6;
        else
            if (val >> 5)
                return 5;
            else
                return 4;
    else
        if (val >> 2)
            if (val >> 3)
                return 3;
            else
                return 2;
        else
            if (val >> 1)
                return 1;
            else
                return 0;
}
int crude_zero_count(uint8_t val) {
    if (val) {
        int count = 0;
        while (((val >> count) & 0x1) == 0) {
            ++count;
        }
        return count;
    }
    else {
        return 8;
    }
}

int wheel_putchar(unsigned char c, int x, int y, int color, bool is_fixed, int scale, bool in_small_font) {
    int font_offset;
    if (in_small_font)
        font_offset = ALT_FONT_OFFSET;
    else
        font_offset = SYSTEM_FONT_OFFSET;
    if (is_fixed) {
        for (int i = 0; i < FONT_SIZE_PER_CHAR; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (((*(pWheel->ram))[font_offset + c * FONT_SIZE_PER_CHAR + i] >> j) & 0x1) {
                    sized_pix(x + j * scale, y + i * scale, scale, color);
                }
            }
        }
        return peek(font_offset + FONT_PARAM_OFFSET_RELATIVE) * scale;
    }
    else {
        uint8_t chr[8];
        int max_bit = 0;
        int min_bit = 7;
        bool zero_flag = true;
        for (int i = 0; i < FONT_SIZE_PER_CHAR; ++i) {
            chr[i] = (*(pWheel->ram))[font_offset + c * FONT_SIZE_PER_CHAR + i];
            if (chr[i] != 0) zero_flag = false;
            max_bit = (((1u << (max_bit + 1)) - 1) > chr[i] ? max_bit : crude_log2(chr[i])); //unfinished
            min_bit = (min_bit < crude_zero_count(chr[i]) ? min_bit : crude_zero_count(chr[i]));
        }
        for (int i = 0; i < FONT_SIZE_PER_CHAR; ++i) {
            for (int j = 0; j < 8 - min_bit; ++j) {
                if ((chr[i] >> (j + min_bit)) & 0x1) {
                    sized_pix(x + j * scale, y + i * scale, scale, color);
                }
            }
        }
        return scale * (zero_flag ? peek(font_offset + FONT_PARAM_OFFSET_RELATIVE) - 2 : max_bit - min_bit + 2);
    }

}
int print(const std::string& text, int x, int y, int color_id, bool is_fixed, int scale, bool in_small_font) {
    int font_offset;
    if (in_small_font)
        font_offset = ALT_FONT_OFFSET;
    else
        font_offset = SYSTEM_FONT_OFFSET;

    int text_width = 0;
    int x_offset = 0;
    int chr_height = peek(font_offset + FONT_PARAM_OFFSET_RELATIVE + 1) * scale;

    for (auto c : text) {
        if (c == '\n') {
            text_width = (text_width > x_offset ? text_width : x_offset);
            x_offset = 0;
            y += chr_height;
            continue;
        }
        x_offset += wheel_putchar(c, x + x_offset, y, color_id, is_fixed, scale, in_small_font);
    }
    text_width = (text_width > x_offset ? text_width : x_offset);
    return text_width;
}

#define FOR_SPR for (int i = 0; i < SPRITE_HEIGHT; ++i) for (int j = 0; j < SPRITE_WIDTH; ++j)
#define SPR_PIX(id,x,y) peek4((TILES_OFFSET + SPRITE_DATA_SIZE * (id)) * 2 + (y) * SPRITE_WIDTH + (x))
void spr_mono_unsafe(int sprite_id, int x, int y, int trans_color, int scale, int flip, int rotate) {
    //rotate first
    int pix_color;
    switch (rotate * 4 + flip) {
        default:
        case 0:
        case 11:
            FOR_SPR {
                if (SPR_PIX(sprite_id, j, i) != trans_color)
                    sized_pix(x + j * scale, y + i * scale, scale, SPR_PIX(sprite_id, j, i));
            }
            break;
        case 1:
        case 10:
            FOR_SPR {
                if (SPR_PIX(sprite_id, SPRITE_WIDTH - j - 1, i) != trans_color)
                    sized_pix(x + j * scale, y + i * scale, scale, SPR_PIX(sprite_id, SPRITE_WIDTH - j - 1, i));
            }
            break;
        case 2:
        case 9:
            FOR_SPR {
                if (SPR_PIX(sprite_id, j, SPRITE_HEIGHT - i - 1) != trans_color)
                    sized_pix(x + j * scale, y + i * scale, scale, SPR_PIX(sprite_id, j, SPRITE_HEIGHT - i - 1));
            }
            break;
        case 3:
        case 8:
            FOR_SPR {
                if (SPR_PIX(sprite_id, SPRITE_WIDTH - j - 1, SPRITE_HEIGHT - i - 1) != trans_color)
                sized_pix(x + j * scale, y + i * scale, scale, SPR_PIX(sprite_id, SPRITE_WIDTH - j - 1, SPRITE_HEIGHT - i - 1));
            }
            break;
        case 4:
        case 15:
            FOR_SPR {
                if (SPR_PIX(sprite_id, SPRITE_WIDTH - j - 1, i) != trans_color)
                    sized_pix(x + i * scale, y + j * scale, scale, SPR_PIX(sprite_id, SPRITE_WIDTH - j - 1, i));
            }
            break;
        case 5:
        case 14:
            FOR_SPR {
                if (SPR_PIX(sprite_id, SPRITE_WIDTH - j - 1, SPRITE_HEIGHT - i - 1) != trans_color)
                    sized_pix(x + i * scale, y + j * scale, scale, SPR_PIX(sprite_id, SPRITE_WIDTH - j - 1, SPRITE_HEIGHT - i - 1));
            }
            break;
        case 6:
        case 13:
            FOR_SPR {
                if (SPR_PIX(sprite_id, j, i) != trans_color)
                    sized_pix(x + i * scale, y + j * scale, scale, SPR_PIX(sprite_id, j, i));
            }
            break;
        case 7:
        case 12:
            FOR_SPR {
                if (SPR_PIX(sprite_id, j, SPRITE_HEIGHT - i - 1) != trans_color)
                    sized_pix(x + i * scale, y + j * scale, scale, SPR_PIX(sprite_id, j, SPRITE_HEIGHT - i - 1));
            }
            break;
    }
}
#undef FOR_SPR
#undef SPR_PIX

void spr(int sprite_id, int x, int y, int trans_color, int scale, int flip, int rotate, int w, int h) {
	//clamp width, height
    if (sprite_id % SPRITE_CANVAS_WIDTH + w > SPRITE_CANVAS_WIDTH)
        w = SPRITE_CANVAS_WIDTH - sprite_id % SPRITE_CANVAS_WIDTH;
    for (int i = 0; i < h; ++i) {
        if (sprite_id + h * SPRITE_CANVAS_WIDTH > SPRITE_CANVAS_WIDTH * SPRITE_CANVAS_HEIGHT)
            break;
        for (int j = 0; j < w; ++j) {
            switch (rotate * 4 + flip) {
                default:
                case 0:
                case 11:
                    spr_mono_unsafe(sprite_id + i * SPRITE_CANVAS_WIDTH + j, x + j * SPRITE_WIDTH * scale, y + i * SPRITE_HEIGHT * scale, trans_color, scale, flip, rotate);
                    break;
                case 1:
                case 10:
                    spr_mono_unsafe(sprite_id + i * SPRITE_CANVAS_WIDTH + j, x + (w - j - 1) * SPRITE_WIDTH * scale, y + i * SPRITE_HEIGHT * scale, trans_color, scale, flip, rotate);
                    break;
                case 2:
                case 9:
                    spr_mono_unsafe(sprite_id + i * SPRITE_CANVAS_WIDTH + j, x + j * SPRITE_WIDTH * scale, y + (h - i - 1) * SPRITE_HEIGHT * scale, trans_color, scale, flip, rotate);
                    break;
                case 3:
                case 8:
                    spr_mono_unsafe(sprite_id + i * SPRITE_CANVAS_WIDTH + j, x + (w - j - 1) * SPRITE_WIDTH * scale, y + (h - i - 1) * SPRITE_HEIGHT * scale, trans_color, scale, flip, rotate);
                    break;
                case 4:
                case 15:
                    spr_mono_unsafe(sprite_id + i * SPRITE_CANVAS_WIDTH + j, x + i * SPRITE_HEIGHT * scale, y + (w - j - 1) * SPRITE_WIDTH * scale, trans_color, scale, flip, rotate);
                    break;
                case 5:
                case 14:
                    spr_mono_unsafe(sprite_id + i * SPRITE_CANVAS_WIDTH + j, x + (h - i - 1) * SPRITE_HEIGHT * scale, y + (w - j - 1) * SPRITE_WIDTH * scale, trans_color, scale, flip, rotate);
                    break;
                case 6:
                case 13:
                    spr_mono_unsafe(sprite_id + i * SPRITE_CANVAS_WIDTH + j, x + i * SPRITE_HEIGHT * scale, y + j * SPRITE_WIDTH * scale, trans_color, scale, flip, rotate);
                    break;
                case 7:
                case 12:
                    spr_mono_unsafe(sprite_id + i * SPRITE_CANVAS_WIDTH + j, x + (h - i - 1) * SPRITE_HEIGHT * scale, y + j * SPRITE_WIDTH * scale, trans_color, scale, flip, rotate);
                    break;
            }
        }
    }
}

bool btn(int btn_code) {
	return peek(GAMEPADS_OFFSET + btn_code / 8) & (1u << (btn_code % 8));
}
bool btnp(int btn_code) {
	return pWheel->btnp_flags[btn_code / 8] & (1u << (btn_code % 8));
}


bool key(int key_code) {
	for (int i = KEYBOARD_OFFSET; i < KEYBOARD_OFFSET + KEYBOARD_SIZE; ++i) {
		if (peek(i) == key_code) return true;
	}
	return false;
}
bool keyp(int key_code) {
	for (int i = 0; i < KEYBOARD_SIZE; ++i) {
		if (pWheel->keyp_flags & (1u << i) && peek(KEYBOARD_OFFSET + i) == key_code) return true;
	}
	return false;
}

int mouse(int data_id) {
	switch (data_id) {
	case namespace_mouse::mouse_x:
		return peek(MOUSE_OFFSET);
	case namespace_mouse::mouse_y:
		return peek(MOUSE_OFFSET + 1);
	case namespace_mouse::mouse_left:
		return peek(MOUSE_OFFSET + 2) & 0b1;
	case namespace_mouse::mouse_middle:
		return (peek(MOUSE_OFFSET + 2) & 0b10) >> 1;
	case namespace_mouse::mouse_right:
		return (peek(MOUSE_OFFSET + 2) & 0b100) >> 2;
	case namespace_mouse::mouse_scroll_x:
		return (peek(MOUSE_OFFSET + 2) >> 3) & ((peek(MOUSE_OFFSET + 3) & 0b1) << 5);
	case namespace_mouse::mouse_scroll_y:
		return (peek(MOUSE_OFFSET + 3) >> 1) & 0b111111;
	default:
		return -1;
	}
}



unsigned long long time() {
    return pWheel->timer;
}
unsigned long long tstamp() {
    return time(nullptr);
}
