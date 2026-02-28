#include "ShanwanGamepad.hpp"
#include <sys/epoll.h>

ShanWanGamepad::ShanWanGamepad(const std::string& dev_fn)
    : dev_fn(dev_fn), jsdev(-1)
{
}

ShanWanGamepad::~ShanWanGamepad()
{
    if (jsdev >= 0) {
        close(jsdev);
    }
}

bool ShanWanGamepad::init()
{
    if (access(dev_fn.c_str(), F_OK) == -1) {
        std::cerr << "Gamepad device not found: " << dev_fn << std::endl;
        return false;
    }

    jsdev = open(dev_fn.c_str(), O_RDONLY | O_NONBLOCK);
    if (jsdev < 0) {
        std::cerr << "Failed to open gamepad: " << dev_fn << std::endl;
        return false;
    }

    std::cout << "✅ Gamepad connected: " << dev_fn << std::endl;
    return true;
}

bool ShanWanGamepad::poll()
{
    if (jsdev < 0) return false;

    js_event ev;
    ssize_t bytes = read(jsdev, &ev, sizeof(ev));

    if (bytes != sizeof(ev)) {
        return false;
    }

    if (ev.type & JS_EVENT_INIT) {
        return false;
    }

    if (ev.type == JS_EVENT_BUTTON) {
        switch (ev.number) {
            case 0: gamepad_input.button_a = ev.value; break;
            case 1: gamepad_input.button_b = ev.value; break;
            case 2: gamepad_input.button_x = ev.value; break;
            case 3: gamepad_input.button_y = ev.value; break;
            case 4: gamepad_input.button_l1 = ev.value; break;
            case 5: gamepad_input.button_r1 = ev.value; break;
            case 6: gamepad_input.button_select = ev.value; break;
            case 7: gamepad_input.button_start = ev.value; break;
            case 8: gamepad_input.button_home = ev.value; break;
            case 9: gamepad_input.button_l3 = ev.value; break;
            case 10: gamepad_input.button_r3 = ev.value; break;
        }
    }

    if (ev.type == JS_EVENT_AXIS) {
        float axis_val = ev.value / 32767.0f;
        
        switch (ev.number) {
            case 0: gamepad_input.analog_stick_left.x = axis_val; break;
            case 1: gamepad_input.analog_stick_left.y = -axis_val; break;
            case 2: gamepad_input.button_l2 = (axis_val == 1); break;
            case 3: gamepad_input.analog_stick_right.x = axis_val; break;
            case 4: gamepad_input.analog_stick_right.y = -axis_val; break;
            case 5: gamepad_input.button_r2 = (axis_val == 1); break;
            case 6:
                gamepad_input.button_left = (axis_val == -1);
                gamepad_input.button_right = (axis_val == 1);
                break;
            case 7:
                gamepad_input.button_up = (axis_val == -1);
                gamepad_input.button_down = (axis_val == 1);
                break;
        }
    }

    return true;
}

ShanWanGamepadInput ShanWanGamepad::readData()
{
    // Read all pending events
    while (poll()) {
        // Process events
    }
    return gamepad_input;
}
