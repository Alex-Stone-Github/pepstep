#include "pepstep.h"
#include <CNCShield.h>

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
	_motor->step(direction == Forward? CLOCKWISE : COUNTER);
	steps += direction == Forward? 1 : -1;
}
auto pep::CNCShieldMotor::get(double steps_per_unit) -> double {
	return double(steps) / steps_per_unit;
}
auto pep::CNCShieldMotor::set(ScheduleEntry& scheduler, double speed) -> void {
	if (speed > 0) direction = Forward;
	if (speed < 0) direction = Backward;
	auto desired_steps_per_second = (unsigned long)(double(max_steps_per_second) * speed);
	scheduler.reschedule(one_second / desired_steps_per_second);
}

auto pep::CNCShieldMotor::stop(ScheduleEntry& scheduler) -> void {
	scheduler.reschedule(timeout_default);
}
auto pep::CNCShieldMotor::reset() -> void {
	steps = 0;
}
