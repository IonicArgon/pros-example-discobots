#pragma once

#include "api.h"
#include "okapi/api.hpp"
#include <algorithm>
#include <cmath>

const double kP = 0.5;
const double kI = 0.0;
const double kD = 0.0;
const double kIntegralMin = -100;
const double kIntegralMax = 100;
const double kMaxErrorToIntegrate = 0.5;

double pid_calculate(double p_current_val);
void pid_set_target(double p_target);
void pid_reset();