#include "additional/s21_additional_functions.h"

// rounders
void s21_bank_rounding(s21_big_decimal* src) {
  if (s21_get_bit_dec(0, *src) == 1) {
    s21_big_decimal one = {.bits = {1, 0, 0, 0, 0, 0, 0}};
    s21_add_mantissa(*src, one, src);
  }
}

void s21_math_rounding(s21_big_decimal* src, unsigned long mod) {
  if (mod > (unsigned long)4) {
    s21_big_decimal one = {.bits = {1, 0, 0, 0, 0, 0, 0}};
    s21_add_mantissa(*src, one, src);
  }
}

// original decimals functions
int s21_negate(s21_decimal value, s21_decimal* result) {
  round_result error_code = ROUND_OK;

  *result = value;
  result->sign_bit = !(result->sign_bit);

  return (int)error_code;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  round_result error_code = ROUND_OK;

  s21_big_decimal value_big = s21_dec_to_big(value);
  s21_big_decimal result_big;

  s21_set_null(&result_big);

  result_big = value_big;
  int scale = result_big.exp_bits;

  if (scale != 0) {
    int mod = s21_decrease_scale(&result_big, scale, NOT_TO_ROUND);

    if (mod > 0 && result_big.sign_bit) {
      s21_big_decimal one = {.bits = {1, 0, 0, 0, 0, 0, 0}};
      s21_add_mantissa(result_big, one, &result_big);
    }
  }

  s21_big_to_dec(result_big, result);

  return (int)error_code;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  round_result error_code = ROUND_OK;

  s21_big_decimal value_big = s21_dec_to_big(value);
  s21_big_decimal result_big;

  s21_set_null(&result_big);

  result_big = value_big;
  int scale = result_big.exp_bits;

  if (scale != 0) {
    s21_decrease_scale(&result_big, scale, TO_ROUND);
  }

  s21_big_to_dec(result_big, result);

  return error_code;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  round_result error_code = ROUND_OK;

  s21_big_decimal value_big = s21_dec_to_big(value);
  s21_big_decimal result_big;

  s21_set_null(&result_big);

  result_big = value_big;
  int scale = result_big.exp_bits;

  s21_decrease_scale(&result_big, scale, NOT_TO_ROUND);

  s21_big_to_dec(result_big, result);

  return (int)error_code;
}
