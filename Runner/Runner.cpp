#include "Runner.h"

#include "glog/logging.h"

void Runner::runInstance() {
  CHECK(instance) << "instance object is null";
  instance->run();
}

void FunctionRunner::run() {
  if (initializer_) {
    initializer_();
  }
  runnerWrapper_();
}
