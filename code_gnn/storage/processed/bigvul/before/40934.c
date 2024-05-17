json_t *json_string_nocheck(const char *value)
{
    json_string_t *string;

    if(!value)
        return NULL;

    string = jsonp_malloc(sizeof(json_string_t));
    if(!string)
        return NULL;
    json_init(&string->json, JSON_STRING);

    string->value = jsonp_strdup(value);
    if(!string->value) {
        jsonp_free(string);
        return NULL;
    }

    return &string->json;
}
