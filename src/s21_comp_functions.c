#include "additional/s21_additional_functions.h"

// >
int s21_is_greater(s21_decimal num1, s21_decimal num2) {
  // normalizing_func(&num1, &num2);
  eq_operation_result res = EQUAL;

  if (num1.sign_bit == num2.sign_bit) {
    s21_big_decimal num1_big = s21_dec_to_big(num1);
    s21_big_decimal num2_big = s21_dec_to_big(num2);

    s21_equate_scales(&num1_big, &num2_big);

    res = s21_diff_mantissa(num1_big, num2_big);

    if (num1.sign_bit) {  // for minus sign
      if (res == FIRST_BIGGER) {
        res = SECOND_BIGGER;
      } else if (res == SECOND_BIGGER) {
        res = FIRST_BIGGER;
      }
    }
  } else if (s21_is_null_orig(num1) && s21_is_null_orig(num2)) {
    res = EQUAL;
  } else {
    res = !num1.sign_bit ? FIRST_BIGGER : SECOND_BIGGER;
  }

  return (int)(res == FIRST_BIGGER);
}

// <=
int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2) {
  return !(s21_is_greater(num1, num2));
}

// <
int s21_is_less(s21_decimal num1, s21_decimal num2) {
  // normalizing_func(&num1, &num2);
  eq_operation_result res = EQUAL;

  if (num1.sign_bit == num2.sign_bit) {
    s21_big_decimal num1_big = s21_dec_to_big(num1);
    s21_big_decimal num2_big = s21_dec_to_big(num2);

    s21_equate_scales(&num1_big, &num2_big);

    res = s21_diff_mantissa(num1_big, num2_big);

    if (num1.sign_bit) {  // for minus sign
      if (res == FIRST_BIGGER) {
        res = SECOND_BIGGER;
      } else if (res == SECOND_BIGGER) {
        res = FIRST_BIGGER;
      }
    }
  } else if (s21_is_null_orig(num1) && s21_is_null_orig(num2)) {
    res = EQUAL;
  } else {
    res = !num1.sign_bit ? FIRST_BIGGER : SECOND_BIGGER;
  }

  return (int)(res == SECOND_BIGGER);
}

// >=
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2) {
  return !(s21_is_less(num1, num2));
}

// ==
int s21_is_equal(s21_decimal num1, s21_decimal num2) {
  int res = 0;

  if (num1.sign_bit == num2.sign_bit) {
    s21_big_decimal num1_big = s21_dec_to_big(num1);
    s21_big_decimal num2_big = s21_dec_to_big(num2);

    s21_equate_scales(&num1_big, &num2_big);

    res = (s21_diff_mantissa(num1_big, num2_big) == EQUAL);
  } else if (s21_is_null_orig(num1) && s21_is_null_orig(num2)) {
    res = 1;
  }

  return res;
}

// !=
int s21_is_not_equal(s21_decimal num1, s21_decimal num2) {
  int res = 0;

  if (num1.sign_bit == num2.sign_bit) {
    s21_big_decimal num1_big = s21_dec_to_big(num1);
    s21_big_decimal num2_big = s21_dec_to_big(num2);

    s21_equate_scales(&num1_big, &num2_big);

    res = (s21_diff_mantissa(num1_big, num2_big) != EQUAL);
  } else if (!(s21_is_null_orig(num1) && s21_is_null_orig(num2))) {
    res = 1;
  }

  return res;
}
