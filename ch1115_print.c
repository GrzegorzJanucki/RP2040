#include "ch1115_print.h"
#include "ch1115_graphics.h"
#include "ch1115_font.h"

size_t print(char c)
{
  return write(c);
}

size_t println(char c)
{
  size_t n = print(c);
  n += println(c);
  return n;
}







