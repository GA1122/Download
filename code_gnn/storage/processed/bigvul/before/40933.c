static int json_string_equal(json_t *string1, json_t *string2)
{
    return strcmp(json_string_value(string1), json_string_value(string2)) == 0;
}
