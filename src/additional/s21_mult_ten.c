#include "s21_additional_functions.h"

int s21_mult_ten(s21_big_decimal* res) {
  bin_operation_result return_code = BIN_OK;
  s21_big_decimal temp = *res;

  s21_shift_l_mantissa(res, 3);
  s21_shift_l_mantissa(&temp, 1);
  return_code = s21_add_mantissa(temp, *res, res);

  return return_code;
}
