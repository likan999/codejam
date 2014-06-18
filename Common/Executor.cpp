#include "Executor.h"

#include <glog/logging.h>

void Executor::runInstance() {
  CHECK(instance) << "instance object is null";
  instance->run();
}

void FunctionalExecutor::run() {
  if (initializerWrapper_) {
    initializerWrapper_();
  }
  solverWrapper_();
}
