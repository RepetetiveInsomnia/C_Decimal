#include "additional/s21_additional_functions.h"

int s21_sub(s21_decimal num1, s21_decimal num2, s21_decimal* result) {
  bin_operation_result return_code = BIN_OK;

  num2.sign_bit = !(num2.sign_bit);

  return_code = s21_add(num1, num2, result);

  return (int)return_code;
}
