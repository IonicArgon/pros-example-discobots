#include "main.h"
#include "yourstuff/chassis.h"
#include "yourstuff/pid.h"

// robot hardware
pros::Motor left_front(1, pros::E_MOTOR_GEARSET_18, false,
                       pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor left_back(2, pros::E_MOTOR_GEARSET_18, false,
                      pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor right_front(3, pros::E_MOTOR_GEARSET_18, true,
                        pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor right_back(4, pros::E_MOTOR_GEARSET_18, true,
                       pros::E_MOTOR_ENCODER_COUNTS);

Chassis chassis_obj(left_front, left_back, right_front, right_back);

pros::Motor lift_mtr(5, pros::E_MOTOR_GEARSET_18, false,
                     pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intake_mtr(6, pros::E_MOTOR_GEARSET_36, true,
                       pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor gun_shooter(7, pros::E_MOTOR_GEARSET_36, false,
                        pros::E_MOTOR_ENCODER_COUNTS);
pros::Controller master(pros::E_CONTROLLER_MASTER);

// selected auton
int auton_selected = 0;

void select_auton() {
  while (true) {
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
      auton_selected++;
      if (auton_selected > 1) {
        auton_selected = 0;
      }
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
      break;
    }

    pros::delay(50);
  }
}

void initialize() { select_auton(); }

void disabled() {}

void competition_initialize() {}

// //* drive function
// void drive_voltage(int left, int right) {
//   left_front.move_voltage(left);
//   left_back.move_voltage(left);
//   right_front.move_voltage(right);
//   right_back.move_voltage(right);
// }

// //* get encoders function
// int get_encoders(char mode) {
//   switch (mode) {
//   case 'l':
//     return (left_front.get_position() + left_back.get_position()) / 2;
//   case 'r':
//     return (right_front.get_position() + right_back.get_position()) / 2;
//   }

//   return 0;
// }

// //* reset encoders
// void reset_encoders() {
//   left_front.tare_position();
//   left_back.tare_position();
//   right_front.tare_position();
//   right_back.tare_position();
// }

//* max time
int max_time = 5000;
int last_voltage = 0;
int dVoltage = 450;
int targetRange = 15;

//* drive forward
void drive_forward(double target) {
  //* get start time
  int start_time = pros::millis();

  //* reset current time, reset everything else
  int current_time = 0;
  chassis_obj.encoder_reset();
  pid_reset();

  //* set target for pid
  pid_set_target(target);

  //* main control loop
  while (true) {
    //* get current time
    current_time = pros::millis();

    //* get how much we've travelled
    int travel =
        (chassis_obj.get_encoders('l') + chassis_obj.get_encoders('r') / 2);

    //* find voltage
    int voltage = pid_calculate(travel) * 12'000;
    voltage =
        std::clamp(voltage, last_voltage - dVoltage, last_voltage + dVoltage);

    //* set voltage
    chassis_obj.drive_voltage(voltage, voltage);

    //* set last voltage
    last_voltage = voltage;

    //? checking if we've reached the target
    if ((target > travel - targetRange) && (target < travel + targetRange))
      break;
    else if (current_time - start_time >
             max_time) //? checking if we're past time
      break;

    //* delay to not hog resources
    pros::delay(20);
  }

  //* reset stuff
  last_voltage = 0;
  chassis_obj.drive_voltage(0, 0);
  chassis_obj.encoder_reset();
}

void skills() { drive_forward(1095); }

void autonomous() {
  if (auton_selected == 1) {
    skills();
  }
}

int something = 10;
pros::Mutex mutex;

void foo() {
  mutex.take(TIMEOUT_MAX);
  something += 10;
  mutex.give();
}

void bar() {
  mutex.take(TIMEOUT_MAX);
  something += 20;
  mutex.give();
}

void opcontrol() {
  pros::Task foo_task(foo);
  pros::Task bar_task(bar);
}
