#pragma once

#include <functional>
#include <iostream>
#include <memory>

class Executor {
 public:
  virtual ~Executor();
  virtual void run() = 0;
  static void runInstance();

 private:
  // Define it in the solver module.
  static std::unique_ptr<Executor> instance;
};

class FunctionalExecutor : public Executor {
 public:
  template <typename Out, typename... In>
  FunctionalExecutor(Out (*solver)(In...), std::istream& is = std::cin, std::ostream& os = std::cout);

  template <typename Out, typename... In>
  FunctionalExecutor(Out (*solver)(In...), void (*initializer)(), std::istream& is = std::cin, std::ostream& os = std::cout);

  virtual ~FunctionalExecutor();

  virtual void run();

 private:
  template <typename Out, typename... In>
  void setSolverWrapper(Out (*solver)(In...), std::istream& is, std::ostream& os);

 private:
  std::function<void()> initializerWrapper_;
  std::function<void()> solverWrapper_;
};

#include "Executor-impl.h"
