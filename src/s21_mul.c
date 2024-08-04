#include "additional/s21_additional_functions.h"

int s21_mul(s21_decimal num1, s21_decimal num2, s21_decimal* result) {
  bin_operation_result return_code = BIN_OK;

  s21_big_decimal num1_big = s21_dec_to_big(num1);
  s21_big_decimal num2_big = s21_dec_to_big(num2);

  s21_set_null_orig(result);

  s21_big_decimal res_big;

  res_big.exp_bits = num1.exp_bits + num2.exp_bits;
  res_big.sign_bit = (num1.sign_bit != num2.sign_bit);

  s21_mult_mantissa(num1_big, num2_big, &res_big);

  return_code = s21_big_to_dec(res_big, result);

  return return_code;
}
