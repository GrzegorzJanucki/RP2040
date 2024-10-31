/*!
  @file     ER_OLEDM1_CH1115_Print.hpp
  @brief    Base class that provides print() and println() for  ER_OLEDM1_CH1115_PICO library
  @note     Port of arduino built-in print class, G Lyons 2022.
*/
#ifndef ch1115_print_h
#define ch1115_print_h
#pragma once

#include <stdint.h>
#include <stdio.h> // for size_t
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2


size_t print(const char);
size_t print (char);
//   size_t print(int, int = DEC);
//   size_t print(unsigned int, int = DEC);
//   size_t print(long, int = DEC);
//   size_t print(unsigned long, int = DEC);
//   size_t print(double, int = 2);
//   size_t print(const std::string &);

//   size_t println(const char[]);
size_t println(char[]);
//   size_t println(int, int = DEC);
//   size_t println(unsigned int, int = DEC);
//   size_t println(long, int = DEC);
//   size_t println(unsigned long, int = DEC);
//   size_t println(double, int = 2);
//   size_t println(void);
//   size_t println(const std::string &s);

#endif