#include "test.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

const char* current_case_name;
int current_case_state;

void test_case_start(const char* name)
{
    current_case_name = name;
    current_case_state = 0;
}

void test_case_end()
{

    if (current_case_state == 0) printf("[SUCCESS] ");
    else printf("[FAILED]  ");

    printf("Test case \"%s\" \n", current_case_name);
}

void test_check_array(const void* arr, const void* expected, size_t size, size_t elem_size)
{
    current_case_state = memcmp(arr, expected, size);
}
