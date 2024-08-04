#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_DECIMAL powl(2.0, 96.0) - 1.0
#define MIN_DECIMAL -1.0 * (MAX_DECIMAL)

#define MAX_EXP 28

#define MAX_BITS_BIG 192
#define MANT_SIZE_BIG 6
#define SIZE_BIG 7

#define MAX_BITS 96
#define MANT_SIZE 3
#define SIZE_ORIG 4

#define IS_VALID(a) \
  (a & 0x3F00FFFF) == 0  // check if zero_bits in last int are zeros
#define MAX_MANTISSA_BIT __UINT32_MAX__

typedef enum { BIN_OK = 0, BIG, SMALL, ZERO_DIV } bin_operation_result;

typedef enum { ROUND_OK = 0, CALC_ERROR } round_result;

typedef enum { CONVERT_OK = 0, CONVERT_ERROR } convert_result;

typedef enum { EQUAL = 0, FIRST_BIGGER, SECOND_BIGGER } eq_operation_result;

typedef union {
  struct {
    unsigned mantissa[3];       // 96 mantissa bits
    unsigned zero_bits16 : 16;  // 16 zero bits
    unsigned exp_bits : 8;      // 8 exponenta bits (num 28 max)
    unsigned zero_bits7 : 7;    // 7 zero bits
    unsigned sign_bit : 1;      // 1 sign bit
  };
  int bits[4];
} s21_decimal;

typedef union {
  struct {
    unsigned mantissa[6];    // 192 mantissa bits
    unsigned exp_bits : 31;  // 31 exponenta bits (num 28 max)
    unsigned sign_bit : 1;
  };
  int bits[7];
} s21_big_decimal;

// operation functions
int s21_add(s21_decimal num1, s21_decimal num2, s21_decimal* result);
int s21_sub(s21_decimal num1, s21_decimal num2, s21_decimal* result);
int s21_mul(s21_decimal num1, s21_decimal num2, s21_decimal* result);
int s21_div(s21_decimal num1, s21_decimal num2, s21_decimal* result);

// rounding functions
int s21_floor(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);

// comparing functions
int s21_is_greater(s21_decimal num1, s21_decimal num2);
int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_less(s21_decimal num1, s21_decimal num2);
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_equal(s21_decimal num1, s21_decimal num2);
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);

// convertors
int s21_from_decimal_to_int(s21_decimal src, int* dst);
int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);
int s21_from_float_to_decimal(float src, s21_decimal* dst);

#endif  // SRC_S21_DECIMAL_H_
