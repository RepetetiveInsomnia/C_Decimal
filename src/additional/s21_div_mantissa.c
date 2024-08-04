#include "s21_additional_functions.h"

s21_big_decimal s21_div_mantissa(s21_big_decimal num1, s21_big_decimal num2,
                                 s21_big_decimal* res) {
  s21_big_decimal mod;

  s21_big_decimal to_sum;
  s21_big_decimal new_num1 = num1;

  s21_set_null(&mod);
  s21_set_null(&to_sum);
  s21_set_null(res);

  // CHECK FOR NUM2 == NULL OUTSIDE!!!!!
  if (!s21_is_null(num2) && !s21_is_null(num1)) {
    // if last result of (new_num1 = new_num1 - to_div) < num2
    // so we have result and mod = new_num1
    while (s21_diff_mantissa(new_num1, num2) != SECOND_BIGGER) {
      s21_big_decimal to_div = num2;  // num2 to shift
      s21_big_decimal one_to_shift = {.bits = {1, 0, 0, 0}};
      int times_shifted = 0;

      // after loop to_div = num2 * 2^times_shifted >= new_num1
      while (s21_diff_mantissa(new_num1, to_div) != SECOND_BIGGER) {
        s21_shift_l_mantissa(&to_div, 1);
        times_shifted++;
      }

      // get power of 2 and number to div for condition: num2 * 2^times_shifted
      // < num1 install values before last iteration
      if (times_shifted) {
        s21_shift_r_mantissa(&to_div, 1);
        times_shifted--;
      }

      // get 2^times_shifted
      s21_shift_l_mantissa(&one_to_shift, times_shifted);

      // res = res + 2^times_shifted
      s21_add_mantissa(*res, one_to_shift, res);

      // next num1 value to check if division is still needed
      // new_num1 = num1 - to_div (at firs iteration)
      s21_sub_mantissa(new_num1, to_div, &new_num1);
    }

    mod = new_num1;
  }

  return mod;
}
