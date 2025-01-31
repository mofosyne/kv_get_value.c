#ifndef KV_FILE_PARSE_VALUE_H
#define KV_FILE_PARSE_VALUE_H
#include <stdio.h>
/**
 * @brief Parses a key-value pair from a given file.
 *
 * 2025 Brian Khuu https://briankhuu.com/ (This function is dedicated to Public Domain)
 * https://briankhuu.com/blog/2025/01/30/simple-key-value-text-parser-in-c/
 *
 * This function extracts the value associated with a specified key in a formatted key-value file.
 *
 * ## Supported Features:
 * - **Whitespace Skipping** (`KV_PARSE_WHITESPACE_SKIP`): Ignores spaces and tabs around keys and values.
 * - **Quoted String Support** (`KV_PARSE_QUOTED_STRINGS`): Handles values enclosed in single (`'`) or double (`"`) quotes.
 * - **Key Delimiters**: Supports both `=` and `:` as key-value separators.
 *
 * @note If `KV_PARSE_WHITESPACE_SKIP` is defined, leading and trailing whitespace is ignored before returning the value.
 * @note If `KV_PARSE_QUOTED_STRINGS` is defined, values can be enclosed in single (`'`) or double (`"`) quotes.
 *
 * @param file Input file stream containing key-value pairs (e.g., "key=value") separated by newline.
 * @param key The key to search for in the input file.
 * @param value Output buffer to store the extracted value.
 * @param value_max Maximum length of the output buffer (`value`), including the null terminator.
 * @return The length of the extracted value (excluding the null terminator) on success, or `0` if the key is not found.
 *
 * @example Usage Example:
 * @code
 * char buffer[50];
 * FILE *temp = tmpfile();
 * fputs("username=admin\npassword=1234", temp);
 * rewind(temp);
 * unsigned int len = kv_parse_value(temp, "username", buffer, sizeof(buffer));
 * if (len > 0) {
 *     printf("Username: %s\n", buffer);
 * } else {
 *     printf("Key not found.\n");
 * }
 * @endcode
 */
unsigned int kv_file_parse_value(FILE *file, const char *key, char *value, unsigned int value_max);
#endif