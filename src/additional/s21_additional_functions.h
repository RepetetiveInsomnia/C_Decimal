#ifndef SRC_ADDITIONAL_S21_ADDITIONAL_FUNCTIONS_H_
#define SRC_ADDITIONAL_S21_ADDITIONAL_FUNCTIONS_H_

#include "../s21_decimal.h"

#define TO_ROUND 1
#define NOT_TO_ROUND 0
// __________________________________________
//            F O R    B I G
// __________________________________________
s21_big_decimal s21_dec_to_big(s21_decimal dec);
int s21_big_to_dec(s21_big_decimal big_dec, s21_decimal* dec);

// bits interval from 0 to 31
void s21_set_bit_int(unsigned bit, unsigned n, int* num);

// bits interval from 0 to 192
void s21_set_bit_dec(unsigned bit, unsigned n, s21_big_decimal* num);

// bits interval from 0 to 31
int s21_get_bit_int(unsigned n, int num);

// bits interval from 0 to 128
int s21_get_bit_dec(unsigned n, s21_big_decimal num);

void s21_set_null(s21_big_decimal* num);
void s21_set_null_exp(s21_big_decimal* num);
void s21_set_null_mant(s21_big_decimal* num);
void s21_set_max(s21_big_decimal* num);

int s21_is_null(s21_big_decimal num);
int s21_is_max_orig_mant(s21_big_decimal num);

// shift left and right n bits
void s21_shift_l_mantissa(s21_big_decimal* num, unsigned n);
void s21_shift_r_mantissa(s21_big_decimal* num, unsigned n);

int s21_get_left_pos(s21_big_decimal num);

// only for rescaled values with same sign !!!
// in sub num1 ALWAYS >= than num2
int s21_add_mantissa(s21_big_decimal num1, s21_big_decimal num2,
                     s21_big_decimal* res);
int s21_sub_mantissa(s21_big_decimal num1, s21_big_decimal num2,
                     s21_big_decimal* res);
int s21_mult_mantissa(s21_big_decimal num1, s21_big_decimal num2,
                      s21_big_decimal* res);
s21_big_decimal s21_div_mantissa(s21_big_decimal num1, s21_big_decimal num2,
                                 s21_big_decimal* res);

// scales
void s21_increase_scale(s21_big_decimal* num, int value);
int s21_decrease_scale(s21_big_decimal* num, int value, int to_round);
void s21_equate_scales(s21_big_decimal* num1, s21_big_decimal* num2);

// comparing
int s21_diff_mantissa(s21_big_decimal num1, s21_big_decimal num2);

// rounding
void s21_bank_rounding(s21_big_decimal* src);
void s21_math_rounding(s21_big_decimal* src, unsigned long mod);

int s21_mult_ten(s21_big_decimal* res);
int s21_div_ten(s21_big_decimal* res);

// __________________________________________
//            F O R    O R I G
// __________________________________________

// bits interval from 0 to 128
int s21_get_bit_dec_orig(unsigned n, s21_decimal num);

int s21_is_null_orig(s21_decimal num);

void s21_set_max_orig(s21_decimal* num);

void s21_set_null_orig(s21_decimal* num);

#endif  // SRC_ADDITIONAL_S21_ADDITIONAL_FUNCTIONS_H_
