#pragma once

namespace pep {

class ScheduleEntry {
  unsigned long call_every;
  unsigned long call_count = 0;
  void (*callback)(void*);
  void* params;

public:
  ScheduleEntry(unsigned int call_every, void (*callback)(void*), void* params);
  auto reschedule(unsigned int call_every) -> void;
  auto poll(unsigned long ellapsed) -> void;
};


constexpr unsigned long one_second = 10000;
class CNCShieldMotor {
  int max_steps_per_second;
  int steps = 0;
  void* motor;
public:
  int step_dir = 0;
  explicit CNCShieldMotor(void* motor, int max_steps_per_second);
  auto step() -> void; // implicit cast of this to void*
  auto get(double steps_per_unit) -> double;
  auto set(ScheduleEntry& scheduler, double speed) -> void;
  auto stop(ScheduleEntry& scheduler) -> void;
  auto reset() -> void;
};

const unsigned long timeout_default = 10000;
}

// Conveniece Macros
#define CNCSMEntry(X) pep::ScheduleEntry(pep::timeout_default, (void(*)(void*))(&pep::CNCShieldMotor::step), (void*)&X)


#define POLL(X) for (auto& entry : X) {entry.poll(millis());}
