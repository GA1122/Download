static json_t *json_string_copy(const json_t *string)
{
    return json_string_nocheck(json_string_value(string));
}
