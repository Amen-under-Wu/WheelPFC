#pragma once
#include <cstdint>
#include <string>

//callbacks

void init();
void update();
void scanline(int i);
//void overlay()

//memory manipulations
void memcpy(uint32_t addr_to, uint32_t addr_from, uint32_t length);
void memset(uint32_t addr, uint8_t val, uint32_t length);
uint32_t pmem(int index);
void pmem(int index, uint32_t val);
void poke(uint32_t addr, uint8_t val);
void poke4(uint32_t addr_4, uint8_t val);
uint8_t peek(uint32_t addr);
uint8_t peek4(uint32_t addr_4);
//void sync(uint8_t mask = 0, int bank = 0, bool to_cart = false);
void vbank(int id);

//drawing functions
void circ(int x, int y, int r, int color_id);
void circb(int x, int y, int r, int color_id);
//void elli(int x, int y, int a, int b, int color_id);
//void ellib(int x, int y, int a, int b, int color_id);
//void clip(int x, int y, int w, int h);
void cls(int color_id = 0);
//int font(const std::string& text, int x, int y, int trans_color = -1, char_width = 8, char_height = 8, bool fixed = false, int scale = 1, bool alt = false);
void line(double x1, double y1, double x2, double y2, int color_id);
//void map(int x = 0, int y = 0, int w = 30, int h = 17, int sx = 0, int sy = 0, int trans_color = -1, int scale = 1, void (*remap_func)() = nullptr);
void pix(int x, int y, int color_id);
uint8_t pix(int x, int y);
int print(const std::string& text, int x, int y, int color_id = 15, bool is_fixed = false, int scale = 1, bool in_small_font = false);
void rect(int x1, int y1, int x2, int y2, int color_id);
void rectb(int x1, int y1, int x2, int y2, int color_id);
void spr(int sprite_id, int x, int y, int trans_color = -1, int scale = 1, int flip = 0, int rotate = 0, int w = 1, int h = 1);
//void tri(double x1, double y1, double x2, double y2, double x3, double y3, int color_id);
//void textri(double x1, double y1, double x2, double y2, double x3, double y3, double u1, double v1, double u2, double v2, double u3, double v3, bool use_map = false, int trans_color = -1);

//inputs
bool btn(int btn_code);
bool btnp(int btn_code);
bool key(int key_code);
bool keyp(int key_code);
namespace namespace_mouse {
    enum mouse_data {mouse_x = 0, mouse_y, mouse_left, mouse_middle, mouse_right, mouse_scroll_x, mouse_scroll_y};
}
int mouse(int data_id);

//audios
//void music(int track = -1, int frame = -1, int row = -1, bool loop = true, bool sustain = false], int tempo = -1, int speed = -1);
//void sfx(int id, int note = -1, int duration = -1, int channel = 0, int volume = 15, int speed = 0);

//other utils
//bool fget(int sprite_id, int flag_id);
//void fset(int sprite_id, int flag_id, bool val);
//int mget(int x, int y);
//int mset(int x, int y, int tile_id);

//system
//void exit();
//void reset();
unsigned long long time();
unsigned long long tstamp();
//void trace(const std::string& text);