# kv_parse_value.c

<versionBadge>![Version 0.1.1](https://img.shields.io/badge/version-0.1.1-blue.svg)</versionBadge>
[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CI/CD Status Badge](https://github.com/mofosyne/kv_parse_value.c/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/mofosyne/kv_parse_value/actions)

Simple ANSI C Key Value String Parser

Writeup about this mini project <https://briankhuu.com/blog/2025/01/30/simple-key-value-text-parser-in-c/>

This function searches for a specified key in a formatted key-value string
and extracts its corresponding value into the provided buffer.


```c
unsigned int kv_parse_value(const char *str, const char *key, char *value, unsigned int value_max);
unsigned int kv_file_parse_value(FILE *file, const char *key, char *value, unsigned int value_max);
```

## Supported Features:
  - **Whitespace Skipping** (`KV_PARSE_WHITESPACE_SKIP`): Ignores spaces and tabs around keys and values.
  - **Quoted String Support** (`KV_PARSE_QUOTED_STRINGS`): Handles values enclosed in single (`'`) or double (`"`) quotes.
  - **Key Delimiters**: Supports both `=` and `:` as key-value separators.

# Usage Example:

## kv_parse_value()

```c
char buffer[50];
unsigned int len = kv_parse_value("username=admin\npassword=1234", "username", buffer, sizeof(buffer));
if (len > 0) {
    printf("Username: %s\n", buffer);
} else {
    printf("Key not found.\n");
}
```
## kv_file_parse_value()

```c
char buffer[50];
FILE *temp = tmpfile();
fputs("username=admin\npassword=1234", temp);
rewind(temp);
unsigned int len = kv_file_parse_value(temp, "username", buffer, sizeof(buffer));
if (len > 0) {
    printf("Username: %s\n", buffer);
} else {
    printf("Key not found.\n");
}
```
