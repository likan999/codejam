#pragma once

#include <iostream>
#include <string>

class Comparator {
 public:
  virtual ~Comparator() = default;
  virtual void readAndCompare(std::istream& expectedStream, std::istream& actualStream) = 0;
};

template <typename T>
class TypedComparator : public Comparator {
 public:
  TypedComparator(const std::string& spec);
  virtual void readAndCompare(std::istream& expectedStream, std::istream& actualStream) override;
};

template <>
class TypedComparator<double> : public Comparator {
 public:
  TypedComparator(const std::string& spec);
  virtual void readAndCompare(std::istream& expectedStream, std::istream& actualStream) override;

 private:
  double epsilon_;
};

#include "Comparator-impl.h"
