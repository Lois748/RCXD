#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include <cmath>

#pragma comment(lib, "Xinput.lib")

// Global variables to store the motor speeds
float global_motor_left = 0.0;
float global_motor_right = 0.0;

float L_MOTOR(const XINPUT_STATE& state) {
    int wheel_rotated = state.Gamepad.sThumbLX;
    int gas_pedal = state.Gamepad.sThumbRY;
    float motor_speed = 0.0f;

    if (wheel_rotated < 0) {
        float wheel_percent = ((-wheel_rotated) / 32768.0f);
        float gas_percent = (gas_pedal / 32767.0f) * 100.0f;
        motor_speed = gas_percent * (1.0f - wheel_percent);
        return motor_speed * 2.55f;
    }
    else if (wheel_rotated > 0) {
        float gas_percent = (gas_pedal / 32767.0f) * 100.0f;
        motor_speed = gas_percent;
        return motor_speed * 2.55f;
    }
    else {
        float gas_percent = (gas_pedal / 32767.0f) * 100.0f;
        motor_speed = gas_percent;
        return motor_speed * 2.55f;
    }
}

float R_MOTOR(const XINPUT_STATE& state) {
    int wheel_rotated = state.Gamepad.sThumbLX;
    int gas_pedal = state.Gamepad.sThumbRY;
    float motor_speed = 0.0f;

    if (wheel_rotated < 0) {
        float gas_percent = (gas_pedal / 32767.0f) * 100.0f;
        motor_speed = gas_percent;
        return motor_speed * 2.55f;
    }
    else if (wheel_rotated > 0) {
        float wheel_percent = ((wheel_rotated) / 32767.0f);
        float gas_percent = (gas_pedal / 32767.0f) * 100.0f;
        motor_speed = gas_percent * (1.0f - wheel_percent);
        return motor_speed * 2.55f;
    }
    else {
        float gas_percent = (gas_pedal / 32767.0f) * 100.0f;
        motor_speed = gas_percent;
        return motor_speed * 2.55f;
    }
}

// Function to update the global motor speeds based on the controller input
void updateMotorValues() {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    while (true) {
        DWORD result = XInputGetState(0, &state);
        if (result == ERROR_SUCCESS) {
            float left_motor_speed = L_MOTOR(state);
            float right_motor_speed = R_MOTOR(state);

            // Update the global motor values
            global_motor_left = left_motor_speed;
            global_motor_right = right_motor_speed;

            // Print the motor values for debugging
            std::cout << "Motor Left: " << global_motor_left << " Motor Right: " << global_motor_right << std::endl;
        }
        else {
            std::cout << "Controller not connected." << std::endl;
        }
        Sleep(100); // Poll every 100ms
    }
}
