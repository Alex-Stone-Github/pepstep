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
constexpr unsigned long max_steps_per_second = 300;
class CNCShieldMotor {
  enum Direction {
    Forward,
    Backward,
  };
  Direction direction = Forward;
  int steps = 0;
  void* motor;

public:
  explicit CNCShieldMotor(void* motor);
  auto step() -> void; // implicit cast of this to void*
  auto get(double steps_per_unit) -> double;
  auto set(ScheduleEntry& scheduler, double speed) -> void;
  auto stop(ScheduleEntry& scheduler) -> void;
  auto reset() -> void;
};

}

// Conveniece Macros
const unsigned long timeout_default = 10000;
#define CNCSMEntry(X) pep::ScheduleEntry(timeout_default, (void(*)(void*))(&pep::CNCShieldMotor::step), (void*)&X)


#define POLL(X) for (auto& entry : X) {entry.poll(millis());}
