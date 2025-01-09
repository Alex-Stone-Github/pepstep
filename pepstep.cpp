#include "pepstep.h"
#include <CNCShield.h>
#include <Arduino.h>

pep::ScheduleEntry::ScheduleEntry(unsigned int call_every, void (*callback)(void*), void* params):
    call_every(call_every),
    callback(callback),
    params(params) {}

auto pep::ScheduleEntry::reschedule(unsigned int call_every) -> void {
	this->call_every = call_every;
	call_count = 0;
}

auto pep::ScheduleEntry::poll(unsigned long ellapsed) -> void {
	auto tmp = ellapsed / call_every;
	if (tmp > call_count) {
	  call_count = tmp;
	  callback(params);
	}
}

constexpr unsigned long one_second = 10000;
constexpr unsigned long max_steps_per_second = 300;

pep::CNCShieldMotor::CNCShieldMotor(void* motor): motor((StepperMotor*)motor) {}
auto pep::CNCShieldMotor::step() -> void {
	StepperMotor* _motor = (StepperMotor*)motor;

	Serial.println(step_dir);

	if (step_dir > 0) _motor->step(CLOCKWISE);
	if (step_dir < 0) _motor->step(COUNTER);
	steps += step_dir;
}
auto pep::CNCShieldMotor::get(double steps_per_unit) -> double {
	return double(steps) / steps_per_unit;
}
auto pep::CNCShieldMotor::set(ScheduleEntry& scheduler, double speed) -> void {
	step_dir = 0;
	if (speed > 0)
		step_dir = 1;
	if (speed < 0) {
		step_dir = -1;
		speed *=- 1;
	}
	auto desired_steps_per_second = (unsigned long)(double(max_steps_per_second) * speed);
	scheduler.reschedule(one_second / desired_steps_per_second);
}

auto pep::CNCShieldMotor::stop(ScheduleEntry& scheduler) -> void {
	scheduler.reschedule(timeout_default);
	step_dir = 0;
}
auto pep::CNCShieldMotor::reset() -> void {
	steps = 0;
}
