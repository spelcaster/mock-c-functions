#include "malloc.hpp"
#include <stdlib.h>

#include <iostream>

void call_malloc () {
  void *ptr = malloc(1024);

  std::cout << ptr << std::endl;

  free(ptr);
}
