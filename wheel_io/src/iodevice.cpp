#include <wheel_io/iodevice.hpp>
#include <iostream>
#include <cmath>
#define SHOW_FPS
using namespace std;

My_shader::My_shader(const char* vs, const char* fs, const char* gs) {
    GLuint vertex, fragment, geometry;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    if (gs != nullptr) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gs, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (gs != nullptr) glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (gs != nullptr) glDeleteShader(geometry);
}

inline void My_shader::use() {
    glUseProgram(ID);
}

void My_shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
        }
    }
}

OpenGLDevice::OpenGLDevice() {
    glfwInit();
    window = glfwCreateWindow(1280, 720, "Wheel - Pseudo Fantasy Console (demo)", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    const char* vs =
            "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "out VS_OUT {\n"
            "   vec3 color;\n"
            "} vs_out;\n"
            "void main() {\n"
            "   vs_out.color = aColor;\n"
            "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
            "}\n";
    const char* fs =
            R"(#version 330 core
                out vec4 FragColor;
                in vec3 fColor;
                void main() {
                    FragColor = vec4(fColor, 1.0);
                })";
    const char* gs =
            R"(#version 330 core
                layout (points) in;
                layout (triangle_strip, max_vertices = 4) out;
                in VS_OUT {
                    vec3 color;
                } gs_in[];
                out vec3 fColor;
                uniform vec2 pixel_size;
                void draw_pixel(vec4 position) {
                    fColor = gs_in[0].color;
                    gl_Position = position;
                    EmitVertex();
                    gl_Position = position + vec4(pixel_size.x, 0.0, 0.0, 0.0);
                    EmitVertex();
                    gl_Position = position + vec4(0.0, -pixel_size.y, 0.0, 0.0);
                    EmitVertex();
                    gl_Position = position + vec4(pixel_size.x, -pixel_size.y, 0.0, 0.0);
                    EmitVertex();

                    EndPrimitive();
                }
                void main() {
                    draw_pixel(gl_in[0].gl_Position);
                })";
    wheelShader = new My_shader(vs, fs, gs);
    wheelShader->use();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    window_size_update(width, height);

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    //window_size_update(width, height);
}

void OpenGLDevice::window_size_update(int width, int height) {
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
    int pixel_size = min(width / (SCREEN_WIDTH + 2), height / (SCREEN_HEIGHT + 2));
    pixel_size_pix = pixel_size;
    for (int i = 0; i <= SCREEN_WIDTH; ++i)
        pixel_x[i] = (float)(2 * i * pixel_size - SCREEN_WIDTH * pixel_size) / (width);
    for (int i = 0; i <= SCREEN_HEIGHT; ++i)
        pixel_y[SCREEN_HEIGHT - i] = (float)(2 * i * pixel_size - SCREEN_HEIGHT * pixel_size) / (height);
    for (int i = 0; i < (SCREEN_WIDTH) * (SCREEN_HEIGHT); ++i) {
        int offset = i * 5;
        vertices[offset] = pixel_x[i % SCREEN_WIDTH];
        vertices[offset + 1] = pixel_y[i / SCREEN_WIDTH];

    }
    float pixel_size_x = (float)(2 * pixel_size) / width;
    float pixel_size_y = (float)(2 * pixel_size) / height;
    glUniform2f(glGetUniformLocation(wheelShader->ID, "pixel_size"), pixel_size_x, pixel_size_y);
}

OpenGLDevice::~OpenGLDevice() {
    glfwTerminate();
}

void OpenGLDevice::draw() {

    uint32_t& bgcolor = gbuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
    glClearColor(((bgcolor >> 16) & 0xff) / 255.0f, ((bgcolor >> 8) & 0xff) / 255.0f, (bgcolor & 0xff) / 255.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        uint32_t color = gbuffer[i];
        vertices[i * 5 + 2] = ((color >> 16) & 0xff) / 255.0f;
        vertices[i * 5 + 3] = ((color >> 8) & 0xff) / 255.0f;
        vertices[i * 5 + 4] = (color & 0xff) / 255.0f;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_DYNAMIC_DRAW);
    wheelShader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, SCREEN_HEIGHT * SCREEN_WIDTH);
    glfwSwapBuffers(window);
#ifdef SHOW_FPS
    static int t = 0;
    static clock_t c = clock();
    t += 1;
    if (clock() - c >= CLOCKS_PER_SEC) {
        printf("fps:%d\n", t);
        t = 0;
        c = clock();
    }
#endif
}

extern OpenGLDevice* pGDevice;
extern OpenGLDevice* pIDevice;

#define GET_KEY(wheel_key,glfw_key) if (glfwGetKey(window,(glfw_key)) == GLFW_PRESS) {ibuffer[IBUFFER_KEYBOARD_OFFSET + count] = (wheel_key);++count;if (count >= 4) return;}

void OpenGLDevice::getKeyInput() {
    int count = 0;
    for (int i = 0; i < 26; ++i) {
        GET_KEY(1 + i, GLFW_KEY_A + i);
    }
    for (int i = 0; i < 10; ++i) {
        GET_KEY(27 + i, GLFW_KEY_0);
    }
    GET_KEY(37, GLFW_KEY_MINUS);
    GET_KEY(38, GLFW_KEY_EQUAL);
    GET_KEY(39, GLFW_KEY_LEFT_BRACKET);
    GET_KEY(40, GLFW_KEY_RIGHT_BRACKET);
    GET_KEY(41, GLFW_KEY_BACKSLASH);
    GET_KEY(42, GLFW_KEY_SEMICOLON);
    GET_KEY(43, GLFW_KEY_APOSTROPHE);
    GET_KEY(44, GLFW_KEY_GRAVE_ACCENT);
    GET_KEY(45, GLFW_KEY_COMMA);
    GET_KEY(46, GLFW_KEY_PERIOD);
    GET_KEY(47, GLFW_KEY_SLASH);
    GET_KEY(48, GLFW_KEY_SPACE);
    GET_KEY(49, GLFW_KEY_TAB);
    GET_KEY(50, GLFW_KEY_ENTER);
    GET_KEY(51, GLFW_KEY_BACKSPACE);
    GET_KEY(52, GLFW_KEY_DELETE);
    GET_KEY(53, GLFW_KEY_INSERT);
    GET_KEY(54, GLFW_KEY_PAGE_UP);
    GET_KEY(55, GLFW_KEY_PAGE_DOWN);
    GET_KEY(56, GLFW_KEY_HOME);
    GET_KEY(57, GLFW_KEY_END);
    GET_KEY(58, GLFW_KEY_UP);
    GET_KEY(59, GLFW_KEY_DOWN);
    GET_KEY(60, GLFW_KEY_LEFT);
    GET_KEY(61, GLFW_KEY_RIGHT);
    GET_KEY(62, GLFW_KEY_CAPS_LOCK);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        ibuffer[8 + count] = 63;
        ++count;
        if (count >= 4) return;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        ibuffer[8 + count] = 64;
        ++count;
        if (count >= 4) return;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
        ibuffer[8 + count] = 65;
        ++count;
        if (count >= 4) return;
    }
    GET_KEY(66, GLFW_KEY_ESCAPE);
    for (int i = 0; i < 12; ++i) {
        GET_KEY(67 + i, GLFW_KEY_F1 + i);
    }
    for (int i = 0; i < 10; ++i) {
        GET_KEY(79 + i, GLFW_KEY_KP_0 + i);
    }
    GET_KEY(89, GLFW_KEY_KP_ADD);
    GET_KEY(90, GLFW_KEY_KP_SUBTRACT);
    GET_KEY(91, GLFW_KEY_KP_MULTIPLY);
    GET_KEY(92, GLFW_KEY_KP_DIVIDE);
    GET_KEY(93, GLFW_KEY_KP_ENTER);
    GET_KEY(94, GLFW_KEY_KP_EQUAL);
}

//different from tic-80, the mouse cannot display in the border area since there is no pixel at that place.
//therefore the ram / ibuffer stores the direct value of mouse()
void OpenGLDevice::getMouseInput() {
    uint8_t left, right, middle, x, y;
    left = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0);
    right = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ? 4 : 0);
    middle = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS ? 2 : 0);
    if (!glfwGetWindowAttrib(window, GLFW_HOVERED)) {
        x = 255u;
        y = 255u;
    }
    else {
        double xf, yf;
        glfwGetCursorPos(window, &xf, &yf);
        xf = xf * 2 / window_width - 1;
        yf = 1 - yf * 2 / window_height;
        int x_relative = (xf - pixel_x[0]) / (pixel_x[1] - pixel_x[0]);
        int y_relative = -(yf - pixel_y[0]) / (pixel_y[0] - pixel_y[1]);
        if (x_relative >= SCREEN_WIDTH || x_relative < 0 || y_relative >= SCREEN_HEIGHT || y_relative < 0) {
            x = 255u;
            y = 255u;
        }
        else {
            x = x_relative;
            y = y_relative;
        }
    }
    ibuffer[4] = x;
    ibuffer[5] = y;
    ibuffer[6] &= 0b11111000;
    ibuffer[6] |= (left | middle | right);
    //printf("%x %x %x\n", ibuffer[4], ibuffer[5], ibuffer[6]);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //printf("%f %f\n", xoffset, yoffset);
    int sxi = round(xoffset), syi = round(yoffset);
    uint16_t scroll_data = (((uint8_t(syi) & 0b00111111) << 6) | (uint8_t(sxi) & 0b00111111));
    uint16_t bi_data_byte = (pIDevice->ibuffer[7] << 8) | pIDevice->ibuffer[6];
    bi_data_byte &= 0b1000000000000111;
    bi_data_byte |= scroll_data << 3;
    pIDevice->ibuffer[6] = bi_data_byte & 0xff;
    pIDevice->ibuffer[7] = bi_data_byte >> 8;
}

void OpenGLDevice::getInput() {
    for (auto& i : ibuffer) {
        i = 0;
    }
    glfwPollEvents();
    if (glfwWindowShouldClose(pGDevice->window)) {
        wheelSystemByte |= 0x1;
        return;
    }
    getKeyInput();
    getMouseInput();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    pGDevice->window_size_update(width, height);
}
