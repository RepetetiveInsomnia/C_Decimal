#include "additional/s21_additional_functions.h"

int s21_div(s21_decimal num1, s21_decimal num2, s21_decimal* result) {
  bin_operation_result return_code = BIN_OK;

  s21_set_null_orig(result);

  // check zero div
  if (s21_is_null_orig(num2)) {
    return_code = ZERO_DIV;

    // check if null
  } else if (s21_is_null_orig(num1)) {
    s21_set_null_orig(result);

    // major calcs
  } else {
    int signed_exp;
    s21_big_decimal num1_big = s21_dec_to_big(num1);
    s21_big_decimal num2_big = s21_dec_to_big(num2);

    s21_big_decimal res_big;
    s21_big_decimal div_res;

    s21_set_null(&res_big);
    s21_set_null(&div_res);

    signed_exp = num1.exp_bits - num2.exp_bits;
    res_big.sign_bit = (num1.sign_bit != num2.sign_bit);

    s21_big_decimal dec_mod = num1_big;

    int to_continue = !s21_is_null(dec_mod);

    while (to_continue) {
      dec_mod = s21_div_mantissa(dec_mod, num2_big, &div_res);

      s21_add_mantissa(res_big, div_res, &res_big);

      if (!s21_is_null(dec_mod) || signed_exp < 0) {
        s21_mult_ten(&res_big);
        s21_mult_ten(&dec_mod);
        signed_exp++;
      }

      to_continue = (!s21_is_null(dec_mod) || signed_exp < 0) &&
                    (s21_get_left_pos(res_big) < MAX_BITS) &&
                    (signed_exp <= MAX_EXP);
    }

    if (signed_exp < 0) {
      return_code = (res_big.sign_bit) ? SMALL : BIG;
    } else {
      res_big.exp_bits = (unsigned)signed_exp;
      return_code = s21_big_to_dec(res_big, result);
    }
  }

  return return_code;
}
