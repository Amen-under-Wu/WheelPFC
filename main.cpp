#include <wheel_core/my_wheel.hpp>
#include <wheel_io/iodevice.hpp>

OpenGLDevice* pGDevice = new OpenGLDevice;
OpenGLDevice* pIDevice = pGDevice;
MyWheel* pWheel = new MyWheel(pGDevice, new AudioDevice, pIDevice);

int main() {
    return pWheel->wheelStart();
}