int json_string_set(json_t *json, const char *value)
{
    if(!value || !utf8_check_string(value, -1))
        return -1;

    return json_string_set_nocheck(json, value);
}
