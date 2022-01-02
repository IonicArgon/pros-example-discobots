#include "yourstuff/pid.h"

okapi::EmaFilter filter(0.5);

double m_target = 0.0;
double m_error = 0.0;
double m_last_error = 0.0;
double m_integral = 0.0;
double m_derivative = 0.0;
int m_time = 0;
int m_last_time = 0;

void pid_set_target(double p_target) { m_target = p_target; }

void pid_reset() {
  m_target = 0.0;
  m_error = 0.0;
  m_last_error = 0.0;
  m_integral = 0.0;
  m_derivative = 0.0;
  m_time = 0;
}

double pid_calculate(double p_current_val) {
  //* extra bits
  m_time = pros::millis();
  int dt = m_time - m_last_time;

  //? calculate our error
  m_error = std::copysign(1.0, m_target) - (p_current_val / m_target);

  //? calculate our derivative
  m_derivative = m_last_error - m_error;
  m_derivative = (dt) ? m_derivative / dt
                      : 0.0; //* prevent divide by zero but not necessary
  m_derivative = filter.filter(m_derivative);

  //? check if we should integrate and integrate if we do
  if (std::fabs(m_error) <= kMaxErrorToIntegrate) {
    m_integral += m_error * dt; //* also not necessary
  }

  //? make sure we actually moved so we can integrate
  if (std::copysign(1.0, m_error) != std::copysign(1.0, m_last_error)) {
    m_integral = 0.0;
  }

  //? clamping our integral
  m_integral = std::clamp(m_integral, kIntegralMin, kIntegralMax);

  //? setting our last error
  m_last_error = m_error;
  m_last_time = m_time;

  //? returning our calculated output
  return (m_error * kP) + (m_integral * kI) + (m_derivative * kD);
}
