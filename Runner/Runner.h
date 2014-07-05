#pragma once

#include <functional>
#include <iostream>
#include <memory>

class Runner {
 public:
  virtual ~Runner() = default;

  virtual void run() = 0;

  static void runInstance();

 private:
  // Define it in the solver module.
  static std::unique_ptr<Runner> instance;
};

class FunctionRunner : public Runner {
 public:
  template <typename Out, typename... In>
  FunctionRunner(Out (*runner)(In...), void (*initializer)() = nullptr, std::istream& is = std::cin, std::ostream& os = std::cout);

  virtual ~FunctionRunner() = default;

  virtual void run();

 private:
  std::function<void()> initializer_;
  std::function<void()> runnerWrapper_;
};

#include "Runner-impl.h"
