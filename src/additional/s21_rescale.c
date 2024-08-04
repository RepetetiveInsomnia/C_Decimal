#include "s21_additional_functions.h"

void s21_increase_scale(s21_big_decimal* num, int value) {
  for (int i = 0; i < value; i++) {
    int prev_scale = num->exp_bits;
    s21_mult_ten(num);
    num->exp_bits = prev_scale + 1;
  }
}

int s21_decrease_scale(s21_big_decimal* num, int value, int to_round) {
  unsigned mod = 0;
  int bank_flag = 1;

  for (int i = 0; i < value; i++) {
    mod = s21_div_ten(num);
    if (mod != 0) {
      bank_flag = 0;
    }
    num->exp_bits--;
  }

  if (mod != 5 || num->exp_bits != 0) {
    bank_flag = 0;
  }

  if (to_round == TO_ROUND) {
    if (bank_flag) {
      s21_bank_rounding(num);
    } else {
      s21_math_rounding(num, mod);
    }
  }

  return mod;
}

void s21_equate_scales(s21_big_decimal* num1, s21_big_decimal* num2) {
  int scale_1 = num1->exp_bits;
  int scale_2 = num2->exp_bits;

  if (scale_1 > MAX_EXP) {
    s21_decrease_scale(num1, scale_1 - MAX_EXP, TO_ROUND);
    scale_1 = num1->exp_bits;
  }
  if (scale_2 > MAX_EXP) {
    s21_decrease_scale(num2, scale_2 - MAX_EXP, TO_ROUND);
    scale_1 = num2->exp_bits;
  }

  int count = scale_1 - scale_2;
  if (count > 0) {
    s21_increase_scale(num2, count);
  }
  if (count < 0) {
    s21_increase_scale(num1, count * -1);
  }
}
