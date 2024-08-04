#include "s21_additional_functions.h"

int s21_sub_mantissa(s21_big_decimal num1, s21_big_decimal num2,
                     s21_big_decimal* res) {
  bin_operation_result return_code = BIN_OK;

  s21_set_null(res);

  for (int i = 0; i < MANT_SIZE_BIG; i++) {
    num2.mantissa[i] = ~num2.mantissa[i];
  }

  s21_add_mantissa(num1, num2, res);

  s21_big_decimal one_bit = {.bits = {1, 0, 0, 0, 0, 0, 0}};

  s21_add_mantissa(*res, one_bit, res);

  return return_code;
}
