#include "kv_parse_value.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Test case function
void run_tests()
{
    char buffer[100] = {0};
    int buffer_count = 0;

    // **Test 1: Basic Key-Value Retrieval**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("key1=value1\nkey2=value2", "key1", buffer, sizeof(buffer));
    assert(buffer_count == 6);
    assert(strcmp(buffer, "value1") == 0);
    printf("Test 1 passed\n");

    // **Test 2: Retrieve Last Key**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("a=b\nc=d\ne=f\ng=hello", "g", buffer, sizeof(buffer));
    assert(buffer_count == 5);
    assert(strcmp(buffer, "hello") == 0);
    printf("Test 2 passed\n");

    // **Test 3: Key Not Found**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("a=b\nc=d", "z", buffer, sizeof(buffer));
    assert(buffer_count == 0);
    printf("Test 3 passed\n");

    // **Test 4: Buffer Too Small**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("longkey=longvalue", "longkey", buffer, sizeof(buffer));
    assert(buffer_count == 9);
    assert(strcmp(buffer, "longvalue") == 0);
    printf("Test 4 passed\n");

    // **Test 5: Empty Input**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("", "anykey", buffer, sizeof(buffer));
    assert(buffer_count == 0);
    printf("Test 5 passed\n");

    // **Test 6: Input Without Key-Value Pairs**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("randomtext\nanotherline", "key", buffer, sizeof(buffer));
    // TODO: Whats going on here?
    assert(buffer_count == 0);
    printf("Test 6 passed\n");

    //  **Test 7: Handling Spaces Around Key and Value**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value(" key = value \n next = test ", "key", buffer, sizeof(buffer));
#ifdef KV_PARSE_WHITESPACE_SKIP
    assert(buffer_count == 5);
    assert(strcmp(buffer, "value") == 0);
#else
    assert(buffer_count == 0);
#endif
    printf("Test 7 passed\n");

    // **Test 8: Duplicate Keys (Return First Occurrence)**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("x=1\nx=2\nx=3", "x", buffer, sizeof(buffer));
    assert(buffer_count == 1);
    assert(strcmp(buffer, "1") == 0);
    printf("Test 8 passed\n");

    // **Test 9: Newline Variations (Windows vs. Unix)**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("a=one\r\nb=two", "b", buffer, sizeof(buffer));
    assert(buffer_count == 3);
    assert(strcmp(buffer, "two") == 0);
    printf("Test 9 passed\n");

    // **Test 10: Key With Special Characters**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("user-name=admin\nuser@domain.com=me", "user-name", buffer, sizeof(buffer));
    assert(buffer_count == 5);
    assert(strcmp(buffer, "admin") == 0);
    printf("Test 10 passed\n");

    // **Test 11: Value Containing '='**
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("path=/home/user=data", "path", buffer, sizeof(buffer));
    assert(buffer_count == 15);
    assert(strcmp(buffer, "/home/user=data") == 0);
    printf("Test 11 passed\n");

    // **Test 12: Quoted String **
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("path=\"/home/user=data\"", "path", buffer, sizeof(buffer));
#ifdef KV_PARSE_QUOTED_STRINGS
    assert(buffer_count == 15);
    assert(strcmp(buffer, "/home/user=data") == 0);
#else
    assert(buffer_count == 17);
    assert(strcmp(buffer, "\"/home/user=data\"") == 0);
#endif
    printf("Test 12 passed\n");

    // **Test 13: Uncapped Quoted String **
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("path=\"/home/user=data", "path", buffer, sizeof(buffer));
#ifdef KV_PARSE_QUOTED_STRINGS
    assert(buffer_count == 15);
    assert(strcmp(buffer, "/home/user=data") == 0);
#else
    assert(buffer_count == 16);
    assert(strcmp(buffer, "\"/home/user=data") == 0);
#endif
    printf("Test 13 passed\n");

    // **Test 14: Quoted String With Escaped Quote **
    memset(buffer, 0, sizeof(buffer));
    buffer_count = kv_parse_value("path=\"/home/\\\"user=data\"", "path", buffer, sizeof(buffer));
#ifdef KV_PARSE_QUOTED_STRINGS
    assert(buffer_count == 17);
    assert(strcmp(buffer, "/home/\\\"user=data") == 0);
#else
    assert(buffer_count == 19);
    assert(strcmp(buffer, "\"/home/\\\"user=data\"") == 0);
#endif
    printf("Test 14 passed\n");

    printf("All tests passed successfully!\n");
}

// Run tests in main()
int main()
{
    run_tests();
    return 0;
}
