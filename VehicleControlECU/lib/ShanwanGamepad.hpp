#pragma once
#ifndef __SHANWANGAMEPAD_HPP__
#define __SHANWANGAMEPAD_HPP__

#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <unordered_map>

class ShanWanGamepadInput
{
public:
    struct Vector2 {
        float x = 0.0f;
        float y = 0.0f;
    };
    
    Vector2 analog_stick_left;
    Vector2 analog_stick_right;
    bool button_up = false;
    bool button_down = false;
    bool button_left = false;
    bool button_right = false;
    bool button_a = false;
    bool button_b = false;
    bool button_x = false;
    bool button_y = false;
    bool button_l1 = false;
    bool button_l2 = false;
    bool button_l3 = false;
    bool button_r1 = false;
    bool button_r2 = false;
    bool button_r3 = false;
    bool button_select = false;
    bool button_start = false;
    bool button_home = false;
};

class ShanWanGamepad
{
public:
    ShanWanGamepad(const std::string& dev_fn = "/dev/input/js0");
    ~ShanWanGamepad();

    bool init();
    ShanWanGamepadInput readData();
    bool isConnected() const { return jsdev >= 0; }

private:
    std::string dev_fn;
    int jsdev;
    ShanWanGamepadInput gamepad_input;
    
    std::unordered_map<std::string, float> axis_states;
    std::unordered_map<std::string, int> button_states;
    
    bool poll();
};

#endif
