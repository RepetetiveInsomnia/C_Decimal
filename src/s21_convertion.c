#include "additional/s21_additional_functions.h"

// converters
int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  convert_result error_code = CONVERT_OK;
  *dst = 0;
  int sign = (src.sign_bit == 1) ? -1 : 1;

  s21_decimal result;
  s21_truncate(src, &result);

  s21_big_decimal result_big = s21_dec_to_big(result);

  if (s21_get_left_pos(result_big) > 30) {
    error_code = CONVERT_ERROR;
    *dst = 0;
  } else {
    *dst = sign * result.mantissa[0];
  }

  return (int)error_code;
}

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  convert_result error_code = CONVERT_OK;

  s21_set_null_orig(dst);

  if (src < 0) {
    src = -src;
    dst->sign_bit = 1;
  }
  dst->mantissa[0] = (unsigned)src;

  return (int)error_code;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  convert_result error_code = CONVERT_OK;
  *dst = 0.0;

  unsigned exp = src.exp_bits;

  for (int i = 0; i < MAX_BITS; i++) {
    if (s21_get_bit_dec_orig(i, src)) {
      *dst += pow(2.0, (double)i);
    }
  }

  while (exp) {
    *dst /= 10.0;
    exp--;
  }

  if (src.sign_bit) {
    *dst = *dst * (-1);
  }

  return (int)error_code;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  convert_result error_code = CONVERT_OK;

  s21_set_null_orig(dst);
  // _______________ FIRST IMPLEMENT ____________________
  if (fabs(src) < powl(10.0, -1 * MAX_EXP)) {
    error_code = CONVERT_ERROR;

  } else if ((src > MAX_DECIMAL) || src < (MIN_DECIMAL) || src != src) {
    error_code = CONVERT_ERROR;

  } else {
    s21_big_decimal big_dst = s21_dec_to_big(*dst);
    int exp = 0;

    if (src < 0.0) {
      big_dst.sign_bit = 1;
    }

    long double src_double = fabsl((long double)src);

    while (!(int)src_double && exp <= MAX_EXP) {
      src_double *= 10.0L;
      exp++;
    }

    int i = (exp > 0);

    while (src_double <= MAX_DECIMAL && exp <= MAX_EXP && i < 7) {
      src_double *= 10.0L;
      exp++;
      i++;
    }

    src_double = roundl(src_double);

    for (i = 0; src_double >= powl(10.0, -8) && i < MAX_BITS; i++) {
      src_double = truncl(src_double) / 2;

      if (src_double - truncl(src_double) > powl(10.0, -8)) {
        s21_set_bit_dec(1, i, &big_dst);
      }
    }

    big_dst.exp_bits = exp;

    s21_big_to_dec(big_dst, dst);
  }

  return error_code;

  // _______________ SECOND IMPLEMENT ____________________
  // if (fabs(src) < powl(10.0, -1 * MAX_EXP)) {
  //   error_code = CONVERT_ERROR;

  // } else if ((src > MAX_DECIMAL) || src < (MIN_DECIMAL)) {
  //   error_code = CONVERT_ERROR;

  // } else {

  // double src_double = (double)src;
  // unsigned exp = 0;

  // while (src_double < 1) {
  //   src_double *= 10.0;
  //   exp++;
  // }

  // src_double *= pow(10, 7);
  // exp += 7;
  // big_dst.exp_bits = exp;

  // src_double = round(src_double);

  // double res = trunc(src_double / 2.0);
  // int bit_num = 0;

  // while (res != 0 && res != 1) {
  //   int mod = (int)(src_double - trunc(src_double / 2) * 2);
  //   s21_set_bit_dec(mod, bit_num, &big_dst);
  //   bit_num++;
  //   res = trunc(src_double / 2);
  //   src_double = res;
  // }

  // s21_set_bit_dec((int)res, bit_num, &big_dst);
  // s21_big_to_dec(big_dst, dst);

  // }

  // return (int)error_code;
}
