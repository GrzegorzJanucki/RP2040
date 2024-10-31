#include "ch1115_print.h"
#include "ch1115_graphics.h"
#include "ch1115_font.h"

size_t print(char c)
{
  return write(c);
}

size_t println(char c[])
{
  int len = sizeof(*c);

  return print(c[len]);
}







