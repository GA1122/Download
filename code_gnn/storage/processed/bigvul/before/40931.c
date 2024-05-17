json_t *json_string(const char *value)
{
    if(!value || !utf8_check_string(value, -1))
        return NULL;

    return json_string_nocheck(value);
}
