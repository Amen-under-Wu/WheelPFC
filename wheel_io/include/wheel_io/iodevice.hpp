#pragma once

#include <wheel_core/my_wheel.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow * window);

class My_shader {
public:
    GLuint ID;
    My_shader(const char* vs, const char* fs, const char* gs = nullptr);
    void use();
private:
    void checkCompileErrors(GLuint shader, std::string type);
};

class OpenGLDevice: public GraphicsDevice, public InputDevice{
private:
    GLFWwindow* window;
    int window_width;
    int window_height;
    int pixel_size_pix;
    My_shader* wheelShader;
    GLuint VBO = 0;
    GLuint VAO = 0;
    std::array<GLfloat, SCREEN_WIDTH + 1> pixel_x;
    std::array<GLfloat, SCREEN_HEIGHT + 1> pixel_y;
    GLfloat vertices[SCREEN_WIDTH * SCREEN_HEIGHT * 5] = {};
    void window_size_update(int width, int height);

    void getKeyInput();
    void getMouseInput();
    //void getGamepadInput();
public:
    OpenGLDevice();
    ~OpenGLDevice();
    void draw() override;
    void getInput() override;
    friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};