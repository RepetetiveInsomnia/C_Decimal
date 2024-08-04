#include "additional/s21_additional_functions.h"

int s21_add(s21_decimal num1, s21_decimal num2, s21_decimal* result) {
  bin_operation_result return_code = BIN_OK;

  s21_set_null_orig(result);

  s21_big_decimal num1_big = s21_dec_to_big(num1);
  s21_big_decimal num2_big = s21_dec_to_big(num2);
  s21_big_decimal result_big;

  s21_set_null(&result_big);

  s21_equate_scales(&num1_big, &num2_big);

  result_big.exp_bits = num1_big.exp_bits;

  if (num1.sign_bit == num2.sign_bit) {
    result_big.sign_bit = num1_big.sign_bit;
    s21_add_mantissa(num1_big, num2_big, &result_big);
  } else {
    int first_bigger = s21_diff_mantissa(num1_big, num2_big);
    if (first_bigger == FIRST_BIGGER) {
      s21_sub_mantissa(num1_big, num2_big, &result_big);
      result_big.sign_bit = num1_big.sign_bit;
    } else {
      s21_sub_mantissa(num2_big, num1_big, &result_big);
      result_big.sign_bit = num2_big.sign_bit;
    }
  }

  return_code = s21_big_to_dec(result_big, result);

  return (int)return_code;
}
