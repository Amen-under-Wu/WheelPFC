#include <wheel_core/api.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;

#include <wheel_core/cart.hpp>

class LineDemo: public WheelCartridge {
public:
    const double pi = acos(-1);
    double pi8 = pi / 8;
    double pi2 = pi * 2;

    double t = 0;

    void init() override {

    }

    void update() override {
        if (btn(4)) return;
        cls();
        for (double i = fmod(t, 8); i <= 135; i += 8) {
            line(i, 0, 0, 135 - i, 8);
            line(i, 135, 135, 135 - i, 6);
            t += 0.01;
        }
        for (double i = fmod(t / 16, pi8); i <= pi2; i += pi8) {
            double x = 68 + 32 * cos(i);
            double y = 68 + 32 * cos(i);
            line(135, 0, x, y, 15);
            line(0, 135, x, y, 15);
        }
        line(0,0,135,0,8);
        line(0,0,0,135,8);
        line(135,0,135,135,6);
        line(0,135,135,135,6);
    }

    void scanline(int row) override {

    }
} lineDemo;

class CircbDemo: public WheelCartridge {
public:
    double a = 0;
    int space = 10;
    void update() override {
        cls();
        for (int i = 0; i <= 200; i += space) {
            circb(120 + 80 * sin(a),
                  68 + 40 * cos(a),
                  i + time() / 40 % space,
                  8);
            circb(120 + 80 * sin(a / 2),
                  68 + 40 * cos(a / 2),
                  i + time() / 40 % space,
                  8);
        }
                a += acos(-1)/240;
    }
} circbDemo;

inline double lua_rand() {
    return rand() * (1.0 / (RAND_MAX + 1));
}
inline int lua_rand(int min, int max) {
    return min + rand() % (max - min + 1);
}
inline int lua_rand(int max) {
    return rand() % max + 1;
}

class CircDemo: public WheelCartridge {
public:
    struct Ball {
        int x;
        int y;
        int dx;
        int dy;
        int r;
        int c;
    } balls[51];
    int d = 1;

    void init() override {
        for (int i = 0; i <= 50; ++i) {
            struct Ball ball={.x = lua_rand(10,220),
                    .y = lua_rand(10,126),
                    .dx= lua_rand(1,2) * d,
                    .dy = lua_rand(1,2)*d,
                    .r = lua_rand(6,12),
                    .c = lua_rand(1,6)};
            balls[i] = ball;
            d = d * -1;
        }
    }

    void update() override {
        cls();
        for (auto& b : balls) {
            b.x=b.x+b.dx;
            b.y=b.y+b.dy;
            if (b.x >= 240-b.r) {
                b.x=240-b.r-1;
                b.dx=-b.dx;
            }
            else if (b.x < b.r) {
                b.x=b.r;
                b.dx=-b.dx;
            }

            if (b.y >= 136-b.r) {
                b.y=136-b.r-1;
                b.dy=-b.dy;
            }
            else if (b.y < b.r) {
                b.y=b.r;
                b.dy=-b.dy;
            }
            circ(b.x,b.y,b.r,b.c);
            circ(b.x+b.r/4,b.y-b.r/4,b.r/4,b.c+7);
        }

    }

} circDemo;

void init() {
    circDemo.init();
}

void update() {
    circDemo.update();
}

void scanline(int line) {

}