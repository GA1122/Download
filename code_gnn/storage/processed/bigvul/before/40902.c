json_t *json_integer(json_int_t value)
{
    json_integer_t *integer = jsonp_malloc(sizeof(json_integer_t));
    if(!integer)
        return NULL;
    json_init(&integer->json, JSON_INTEGER);

    integer->value = value;
    return &integer->json;
}
