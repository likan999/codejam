#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE

#include "StreamScanf.h"

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <exception>
#include <functional>
#include <iostream>

#include "glog/logging.h"

using namespace std;

namespace io {

namespace {

struct StreamWrapper {
  istream* inputStream;
  bool finished;
  exception_ptr exception;
};

ssize_t readStream(void *cookie, char *buf, size_t size) {
  CHECK(size == 1);
  StreamWrapper* s = reinterpret_cast<StreamWrapper*>(cookie);
  if (s->finished) {
    return 0;
  }
  istream& is = *s->inputStream;
  istream::int_type c;
  try {
    c = is.get();
  } catch (...) {
    s->exception = current_exception();
    return -1;
  }
  if (is.bad()) {
    return -1;
  } else if (is.eof()) {
    return 0;
  }
  CHECK(c != istream::traits_type::eof());
  *buf = c;
  return 1;
}

class Finally {
 public:
  template <typename Function>
  Finally(Function f) : f_(f) {
  }

  ~Finally() {
    f_();
  }

 private:
  std::function<void()> f_;
};

} // namespace

int streamScanf(istream& is, const char* format, ...) {
  StreamWrapper s;
  s.inputStream = &is;
  s.finished = false;

  cookie_io_functions_t cookieIoFunctions;
  memset(&cookieIoFunctions, 0, sizeof(cookieIoFunctions));
  cookieIoFunctions.read = &readStream;

  // Clear exception mask.
  ios_base::iostate mask = is.exceptions();
  Finally restoreMask([mask, &is]() {
      is.exceptions(mask);
  });
  is.exceptions(ios_base::goodbit);

  // Create the C-style file stream.
  FILE* file = fopencookie(&s, "r", cookieIoFunctions);
  CHECK(file != NULL);
  Finally closeFile([file]() {
      CHECK(fclose(file) == 0) << "Failed to close FILE*: " << strerror(errno);
  });

  // Turn off any buffer.
  setbuf(file, nullptr);

  // Call the actual scanf.
  va_list args;
  va_start(args, format);
  int ret = vfscanf(file, format, args);
  va_end(args);

  // Clear any failbit.
  is.clear(is.rdstate() & ~ios_base::failbit);

  // Put last character back to is.
  s.finished = true;
  int last = fgetc(file);
  if (last != EOF) {
    is.putback(last);
  }

  // If error occurs inside readStream, throw it to outside.
  if (s.exception) {
    rethrow_exception(s.exception);
  }

  return ret;
}

} // namespace io
