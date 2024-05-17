static json_t *json_integer_copy(const json_t *integer)
{
    return json_integer(json_integer_value(integer));
}
