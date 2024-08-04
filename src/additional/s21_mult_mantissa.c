#include "s21_additional_functions.h"

int s21_mult_mantissa(s21_big_decimal num1, s21_big_decimal num2,
                      s21_big_decimal* res) {
  bin_operation_result return_code = BIN_OK;

  s21_set_null_mant(res);

  for (int i = 0; i < MAX_BITS_BIG; i++) {
    int nbit1 = s21_get_bit_dec(i, num1);

    s21_big_decimal temp_num2 = num2;
    // s21_set_null(&temp_num2);

    if (nbit1) {
      // multiplication num2 (copy) by 2^i
      s21_shift_l_mantissa(&temp_num2, i);
      return_code = s21_add_mantissa(*res, temp_num2, res);
    }
  }

  return return_code;
}
