#ifndef TEST_H
#define TEST_H

#include <stddef.h>

void test_case_start(const char* name);
void test_case_end();

void test_check_array(const void* arr, const void* expected, size_t size, size_t elem_size);

#define TEST_CASE(name) test_case_start(name);
#define TEST_CASE_EVAL() test_case_end();

#define TEST_CHECK_ARRAY(arr, expected) \
test_check_array(arr, expected, sizeof(arr) / sizeof((arr)[0]), sizeof((arr)[0]));

#endif /* !TEST_H */
