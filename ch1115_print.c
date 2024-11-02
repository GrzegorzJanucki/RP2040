#include "ch1115_print.h"
#include "ch1115_graphics.h"
#include "ch1115_font.h"

size_t print(char c)
{
  return write(c);
}

size_t println(char* pTable, int len)
{
  for(int i=0; i<len; i++){
    print(pTable[i]);
  }
  return len;
}

size_t println1(char* pTable)
{
  int i = 0;
  do {
    print(pTable[i]);
  } while(pTable[++i] != '\n');
  return i;
}





