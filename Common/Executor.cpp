#include "Executor.h"

#include <glog/logging.h>

Executor::~Executor() {
}

void Executor::runInstance() {
  CHECK(instance) << "instance object is null";
  instance->run();
}

FunctionalExecutor::~FunctionalExecutor() {
}

void FunctionalExecutor::run() {
  if (initializerWrapper_) {
    initializerWrapper_();
  }
  solverWrapper_();
}
