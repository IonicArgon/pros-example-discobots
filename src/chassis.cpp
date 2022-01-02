#include "yourstuff/chassis.h"

Chassis::Chassis(pros::Motor left_front, pros::Motor left_back,
                 pros::Motor right_front, pros::Motor right_back, bool reversed)
    : m_motor_lf(left_front), m_motor_lb(left_back), m_motor_rf(right_front),
      m_motor_rb(right_back) {
  // do nothing
}

void Chassis::drive_velocity(int left, int right) {
  m_motor_lf.move_velocity(left);
  m_motor_lb.move_velocity(left);
  m_motor_rf.move_velocity(right);
  m_motor_rb.move_velocity(right);
}

void Chassis::drive_voltage(int left, int right) {
  m_motor_lf.move_voltage(left);
  m_motor_lb.move_voltage(left);
  m_motor_rf.move_voltage(right);
  m_motor_rb.move_voltage(right);
}

void Chassis::encoder_reset() {
  m_motor_lf.tare_position();
  m_motor_lb.tare_position();
  m_motor_rf.tare_position();
  m_motor_rb.tare_position();
}

int Chassis::get_encoders(char side) {
  switch (side) {
  case 'l':
    return (m_motor_lf.get_position() + m_motor_lb.get_position()) / 2;
  case 'r':
    return (m_motor_rf.get_position() + m_motor_rb.get_position()) / 2;
  }

  return 0;
}