static const char *strip(char *str)
{
    size_t length;
    char *result = str;
    while (*result && l_isspace(*result))
        result++;

    length = strlen(result);
    if (length == 0)
        return result;

    while (l_isspace(result[length - 1]))
        result[--length] = '\0';

    return result;
}
