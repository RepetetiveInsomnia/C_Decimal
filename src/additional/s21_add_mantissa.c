#include "s21_additional_functions.h"

int s21_add_mantissa(s21_big_decimal num1, s21_big_decimal num2,
                     s21_big_decimal* res) {
  bin_operation_result return_code = BIN_OK;
  int overflow = 0;

  s21_set_null(res);

  // setting same exp
  res->exp_bits = num1.exp_bits;
  res->sign_bit = num1.sign_bit;

  for (int i = 0; i < MAX_BITS_BIG; i++) {
    int nbit1 = s21_get_bit_dec(i, num1);
    int nbit2 = s21_get_bit_dec(i, num2);

    int nbit = (nbit1 ^ nbit2) ^ overflow;

    overflow = (nbit1 && nbit2) || (nbit1 && overflow) || (nbit2 && overflow);

    s21_set_bit_dec(nbit, i, res);

    if ((i == MAX_BITS_BIG - 1) && overflow) {
      return_code = BIG;
    }
  }

  return return_code;
}
