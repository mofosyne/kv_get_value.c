#ifndef KV_PARSE_VALUE_H
#define KV_PARSE_VALUE_H
/**
 * @brief Parses a key-value pair from a given string.
 *
 * 2025 Brian Khuu https://briankhuu.com/ (This function is dedicated to Public Domain)
 *
 * This function searches for a specified key in a formatted key-value string
 * and extracts its corresponding value into the provided buffer.
 *
 * ## Supported Features:
 * - **Whitespace Skipping** (`KV_PARSE_WHITESPACE_SKIP`): Ignores spaces and tabs around keys and values.
 * - **Quoted String Support** (`KV_PARSE_QUOTED_STRINGS`): Handles values enclosed in single (`'`) or double (`"`) quotes.
 * - **Key Delimiters**: Supports both `=` and `:` as key-value separators.
 *
 * @param str Input string containing multiple key-value pairs (e.g., `"key=value"`) seperated by newline.
 * @param key The key to search for in the input string.
 * @param value Output buffer to store the extracted value.
 * @param value_max Maximum length of the output buffer (`value`), including the null terminator.
 * @return The length of the extracted value (excluding the null terminator) on success, or `0` if the key is not found.
 *
 * @note If `KV_PARSE_WHITESPACE_SKIP` is defined, leading and trailing whitespace is ignored.
 * @note If `KV_PARSE_QUOTED_STRINGS` is defined, values can be enclosed in single (`'`) or double (`"`) quotes.
 *
 * @example Usage Example:
 * @code
 * char buffer[50];
 * unsigned int len = kv_parse_value("username=admin\npassword=1234", "username", buffer, sizeof(buffer));
 * if (len > 0) {
 *     printf("Username: %s\n", buffer);
 * } else {
 *     printf("Key not found.\n");
 * }
 * @endcode
 */
unsigned int kv_parse_value(const char *str, const char *key, char *value, unsigned int value_max);
#endif
