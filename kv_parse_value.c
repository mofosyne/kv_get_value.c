/* Simple ANSI C KV Parser
 * 2025 Brian Khuu (This Code Is Released To The Public Domain)
 * To test this function run `cat kvparser.h | tcc -DKV_PARSER_TEST -run -` */

/**
 * @brief Parses a key-value pair from a given string.
 *
 * 2025 Brian Khuu https://briankhuu.com/ (This function is dedicated to Public Domain)
 * https://briankhuu.com/blog/2025/01/30/simple-key-value-text-parser-in-c/
 *
 * This function extracts the value associated with a specified key in a formatted key-value string.
 *
 * ## Supported Features:
 * - **Whitespace Skipping** (`KV_PARSE_WHITESPACE_SKIP`): Ignores spaces and tabs around keys and values.
 * - **Quoted String Support** (`KV_PARSE_QUOTED_STRINGS`): Handles values enclosed in single (`'`) or double (`"`) quotes.
 * - **Key Delimiters**: Supports both `=` and `:` as key-value separators.
 *
 * @param str Input string containing multiple key-value pairs (e.g., "key=value") separated by newline.
 * @param key The key to search for in the input string.
 * @param value Output buffer to store the extracted value.
 * @param value_max Maximum length of the output buffer (`value`), including the null terminator.
 * @return The length of the extracted value (excluding the null terminator) on success, or `0` if the key is not found.
 *
 * @note If `KV_PARSE_WHITESPACE_SKIP` is defined, leading and trailing whitespace is ignored before returning the value.
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
unsigned int kv_parse_value(const char *str, const char *key, char *value, unsigned int value_max)
{
    for (; *str != '\0'; str++)
    {
#ifdef KV_PARSE_WHITESPACE_SKIP
        while (*str == ' ' || *str == '\t')
        {
            str++;
        }
#endif

        /* Check For Key */
        for (int i = 0; *str != '\0' && key[i] != '\0'; i++, str++)
        {
            /* Key Mismatched. Skip Line */
            if (*str != key[i])
            {
                goto skip_line;
            }
        }

#ifdef KV_PARSE_WHITESPACE_SKIP
        while (*str == ' ' || *str == '\t')
        {
            str++;
        }
#endif

        /* Check For Key Value Delimiter */
        if (*str != '=' && *str != ':')
        {
            goto skip_line;
        }
        str++;

#ifdef KV_PARSE_WHITESPACE_SKIP
        while (*str == ' ' || *str == '\t')
        {
            str++;
        }
#endif

        /* Copy Value To Buffer */
#ifdef KV_PARSE_QUOTED_STRINGS
        char quote = '\0';
#endif
        for (int i = 0; i < (value_max - 1); str++)
        {
            if (*str == '\0' || *str == '\r' || *str == '\n')
            {
                /* End Of Line. Trim trailing whitespace before returning the value. */
                value[i] = '\0';
#ifdef KV_PARSE_WHITESPACE_SKIP
                while (i > 0 && (value[i - 1] == ' ' || value[i - 1] == '\t'))
                {
                    i--;
                    value[i] = '\0';
                }
#endif
                return i;
            }
#ifdef KV_PARSE_QUOTED_STRINGS
            else if (quote == '\0' && (*str == '\'' || *str == '"'))
            {
                /* Start Of Quoted String */
                quote = *str;
                continue;
            }
            else if (quote != '\0' && *(str - 1) != '\\' && *str == quote)
            {
                /* End Of Quoted String. Return Value */
                value[i] = '\0';
                return i;
            }
            else if (quote != '\0' && *(str - 1) == '\\' && *str == quote)
            {
                /* Escaped Quote Character In Quoted String */
                value[i - 1] = *str;
                continue;
            }
#endif

            value[i++] = *str;
        }

        /* Value too large for buffer. Don't return a value. */
        value[0] = '\0';
        return 0;

    skip_line:
        /* Search for start of next line */
        while (*str != '\n')
        {
            if (*str == '\0')
            {
                /* End of string. Key was not found. */
                return 0;
            }
            str++;
        }
    }

    /* End of string. Key was not found. */
    return 0;
}
