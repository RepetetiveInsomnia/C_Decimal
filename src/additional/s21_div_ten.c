#include "s21_additional_functions.h"

int s21_div_ten(s21_big_decimal* res) {
  unsigned long mod = 0;

  for (int i = MANT_SIZE_BIG - 1; i >= 0; i--) {
    unsigned long temp = (unsigned long)res->mantissa[i] + mod;
    mod = ((unsigned long)(temp % 10)) << 32;
    res->mantissa[i] = temp / 10;
  }

  return (int)(mod >> 32);
}
