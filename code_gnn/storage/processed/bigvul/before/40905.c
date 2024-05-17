int json_integer_set(json_t *json, json_int_t value)
{
    if(!json_is_integer(json))
        return -1;

    json_to_integer(json)->value = value;

    return 0;
}
