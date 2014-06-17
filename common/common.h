#pragma once

#include <iostream>

template <typename Out, typename... In>
void run(Out (*solver)(In...), std::istream& is = std::cin, std::ostream& os = std::cout);

#include "common-impl.h"
