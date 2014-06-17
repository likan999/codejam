#include <glog/logging.h>

#include "Common/Executor.h"

int main(int /*argc*/, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  Executor::runInstance();
}
