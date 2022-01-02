#pragma once

#include "api.h"

class Chassis {
public:
    Chassis(
        pros::Motor left_front,
        pros::Motor left_back,
        pros::Motor right_front,
        pros::Motor right_back,
        bool reversed = false
    );
    
    void drive_velocity(int left, int right);
    void drive_voltage(int left, int right);

    void encoder_reset();
    int get_encoders(char side);

private:
    pros::Motor m_motor_lf;
    pros::Motor m_motor_lb;
    pros::Motor m_motor_rf;
    pros::Motor m_motor_rb;
};