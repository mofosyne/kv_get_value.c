/* Simple ANSI C KV Parser
 * 2025 Brian Khuu (This Code Is Released To The Public Domain) */

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
unsigned int kv_file_parse_value(FILE *file, const char *key, char *value, unsigned int value_max)
{
    for (char ch = getc(file); ch != EOF; ch = getc(file))
    {
#ifdef KV_PARSE_WHITESPACE_SKIP
        while (ch == ' ' || ch == '\t')
        {
            ch = getc(file);
        }
#endif

        /* Check For Key */
        for (int i = 0; ch != EOF && key[i] != '\0'; i++, ch = getc(file))
        {
            /* Key Mismatched. Skip Line */
            if (ch != key[i])
            {
                goto skip_line;
            }
        }

#ifdef KV_PARSE_WHITESPACE_SKIP
        while (ch == ' ' || ch == '\t')
        {
            ch = getc(file);
        }
#endif

        /* Check For Key Value Delimiter */
        if (ch != '=' && ch != ':')
        {
            return 0;
        }
        ch = getc(file);

#ifdef KV_PARSE_WHITESPACE_SKIP
        while (ch == ' ' || ch == '\t')
        {
            ch = getc(file);
        }
#endif

        /* Copy Value To Buffer */
#ifdef KV_PARSE_QUOTED_STRINGS
        int quote = EOF;
        int prev = EOF;
#endif
        for (int i = 0; i < (value_max - 1); ch = getc(file))
        {
            if (ch == EOF || ch == '\r' || ch == '\n')
            {
                /* End Of Line. Return Value */
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
            else if (quote == EOF && (ch == '\'' || ch == '"'))
            {
                /* Start Of Quoted String */
                quote = ch;
                continue;
            }
            else if (quote != EOF && prev != '\\' && ch == quote)
            {
                /* End Of Quoted String. Return Value */
                value[i] = '\0';
                return i;
            }
            else if (quote != EOF && prev == '\\' && ch == quote)
            {
                /* Escaped Character In Quoted String */
                value[i - 1] = ch == EOF ? '\0' : ch;
                continue;
            }

            prev = ch;
#endif

            value[i++] = ch == EOF ? '\0' : ch;
        }

        /* Value too large for buffer. Don't return a value. */
        value[0] = '\0';
        return 0;

    skip_line:
        /* Search for start of next line */
        while (ch != '\n')
        {
            if (ch == EOF)
            {
                /* End of string. Key was not found. */
                return 0;
            }
            ch = getc(file);
        }
    }

    /* End of string. Key was not found. */
    return 0;
}
