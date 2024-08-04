#include "s21_additional_functions.h"
// ------------------------------------
//     B I T    O P E R A T I O N S
// ------------------------------------
// bits interval from 0 to 31
void s21_set_bit_int(unsigned bit, unsigned n, int* num) {
  *num = (bit ? (*num) | (1 << n) : (*num) & ~(1 << n));
}

// bits interval from 0 to 191
void s21_set_bit_dec(unsigned bit, unsigned n, s21_big_decimal* num) {
  s21_set_bit_int(bit, n % 32, &(num->bits[n / 32]));
}

int s21_is_null(s21_big_decimal num) {
  int is_null = 1;

  for (int i = 0; i < MANT_SIZE_BIG; i++) {
    if (num.mantissa[i]) {
      is_null = 0;
    }
  }

  return is_null;
}

int s21_is_max_orig_mant(s21_big_decimal num) {
  int is_max = 1;

  for (int i = 0; i < MANT_SIZE; i++) {
    if (num.mantissa[i] != UINT32_MAX) {
      is_max = 0;
    }
  }

  return is_max;
}

// bits interval from 0 to 31
int s21_get_bit_int(unsigned n, int num) { return (num >> n) & 1; }

// bits interval from 0 to 191
int s21_get_bit_dec(unsigned n, s21_big_decimal num) {
  return s21_get_bit_int(n % 32, num.bits[n / 32]);
}

// set every bit in 0
void s21_set_null(s21_big_decimal* num) {
  for (int i = 0; i < SIZE_BIG; i++) {
    num->bits[i] = 0;
  }
}

// set null exp (only exp!)
void s21_set_null_exp(s21_big_decimal* num) { num->exp_bits = 0; }

// set null mantissa (only mantissa!)
void s21_set_null_mant(s21_big_decimal* num) {
  for (int i = 0; i < MANT_SIZE_BIG; i++) {
    num->mantissa[i] = 0;
  }
}

// set max dec
void s21_set_max(s21_big_decimal* num) {
  for (int i = 0; i < SIZE_BIG; i++) {
    num->bits[i] = (i != SIZE_BIG - 1) ? MAX_MANTISSA_BIT : 0;
  }
}

// num of bits from 0 to 191
int s21_get_left_pos(s21_big_decimal num) {
  int bits = MAX_BITS_BIG - 1;

  while (bits >= 0) {
    if (s21_get_bit_dec(bits, num)) {
      break;
    }
    bits--;
  }

  return bits < 0 ? 0 : bits;
}

// bits interval from 0 to 191
void s21_shift_l_mantissa(s21_big_decimal* num, unsigned n) {
  int shift_bits = n % 32;
  int shift_ints = n / 32;

  for (int i = MANT_SIZE_BIG - 1; i >= shift_ints && shift_ints != 0; i--) {
    num->mantissa[i] = num->mantissa[i - shift_ints];
  }

  for (int i = 0; i < shift_ints && i < MANT_SIZE_BIG; i++) {
    num->mantissa[i] = 0;
  }

  if (shift_bits != 0) {
    unsigned carry = 0;

    for (int i = shift_ints; i < MANT_SIZE_BIG; i++) {
      unsigned temp = num->mantissa[i];
      num->mantissa[i] = (num->mantissa[i] << shift_bits) | carry;
      carry = temp >> (32 - shift_bits);
    }
  }
}

// bits interval from 0 to 95
void s21_shift_r_mantissa(s21_big_decimal* num, unsigned n) {
  int shift_bits = n % 32;
  int shift_ints = n / 32;

  for (int i = 0; i < MANT_SIZE_BIG - shift_ints && shift_ints != 0; i++) {
    num->mantissa[i] = num->mantissa[i + shift_ints];
  }

  for (int i = 0; i < shift_ints && i < MANT_SIZE_BIG; i++) {
    num->mantissa[MANT_SIZE_BIG - 1 - i] = 0;
  }

  if (shift_bits != 0) {
    unsigned carry = 0;

    for (int i = MANT_SIZE_BIG - 1 - shift_ints; i >= 0; i--) {
      unsigned temp = num->mantissa[i];
      num->mantissa[i] = (num->mantissa[i] >> shift_bits) | carry;
      carry = temp << (32 - shift_bits);
    }
  }
}

s21_big_decimal s21_dec_to_big(s21_decimal dec) {
  s21_big_decimal big_dec = {.bits = {0, 0, 0, 0, 0, 0, 0}};

  for (int i = 0; i < MANT_SIZE; i++) {
    big_dec.mantissa[i] = dec.mantissa[i];
  }

  big_dec.exp_bits = dec.exp_bits;
  big_dec.sign_bit = dec.sign_bit;

  return big_dec;
}

int s21_big_to_dec(s21_big_decimal big_dec, s21_decimal* dec) {
  bin_operation_result res = BIN_OK;
  int left_pos = s21_get_left_pos(big_dec);

  s21_set_null_orig(dec);

  unsigned mod = 0;
  int bank_flag = 1;
  int div_ten_continue = (left_pos > MAX_BITS - 1) && (big_dec.exp_bits != 0);

  while (div_ten_continue) {
    mod = s21_div_ten(&big_dec);

    big_dec.exp_bits--;

    left_pos = s21_get_left_pos(big_dec);

    div_ten_continue = (left_pos > MAX_BITS - 1) && (big_dec.exp_bits != 0);

    if (mod != 0 && div_ten_continue) {
      bank_flag = 0;
    }
  }

  if (big_dec.exp_bits > MAX_EXP) {
    int to_decrease = big_dec.exp_bits - MAX_EXP;
    s21_decrease_scale(&big_dec, to_decrease, TO_ROUND);

  } else {
    if (mod != 5 || big_dec.exp_bits != 0) {
      bank_flag = 0;
    }

    if (left_pos > MAX_BITS - 1) {
      res = big_dec.sign_bit ? SMALL : BIG;
    } else if (s21_is_max_orig_mant(big_dec) && mod > 4) {
      if (big_dec.exp_bits > 0) {
        s21_decrease_scale(&big_dec, 1, TO_ROUND);
      } else {
        res = big_dec.sign_bit ? SMALL : BIG;
      }
    } else if (bank_flag) {
      s21_bank_rounding(&big_dec);
    } else {
      s21_math_rounding(&big_dec, mod);
    }
  }

  if (res == BIN_OK) {
    for (int i = 0; i < MANT_SIZE; i++) {
      dec->mantissa[i] = big_dec.mantissa[i];
    }
    dec->exp_bits = big_dec.exp_bits;
    dec->sign_bit = big_dec.sign_bit;
  }

  return (int)res;
}

int s21_get_bit_dec_orig(unsigned n, s21_decimal num) {
  return s21_get_bit_int(n % 32, num.bits[n / 32]);
}

int s21_is_null_orig(s21_decimal num) {
  int is_null = 1;

  for (int i = 0; i < MANT_SIZE; i++) {
    if (num.mantissa[i]) {
      is_null = 0;
    }
  }

  return is_null;
}

// set max dec
void s21_set_max_orig(s21_decimal* num) {
  for (int i = 0; i < SIZE_ORIG; i++) {
    num->bits[i] = (i != SIZE_ORIG - 1) ? MAX_MANTISSA_BIT : 0;
  }
}

void s21_set_null_orig(s21_decimal* num) {
  for (int i = 0; i < SIZE_ORIG; i++) {
    num->bits[i] = 0;
  }
}

// comparing mantissa
int s21_diff_mantissa(s21_big_decimal num1, s21_big_decimal num2) {
  eq_operation_result res = EQUAL;
  int bits = MAX_BITS_BIG - 1;

  while (bits >= 0) {
    int bit1 = s21_get_bit_dec(bits, num1);
    int bit2 = s21_get_bit_dec(bits, num2);
    if ((bit1 ^ bit2) & 1) {
      res = bit1 ? FIRST_BIGGER : SECOND_BIGGER;
      break;
    }
    bits--;
  }
  return (int)res;
}
