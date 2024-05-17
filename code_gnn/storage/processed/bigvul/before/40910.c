static json_t *json_object_copy(json_t *object)
{
    json_t *result;

    const char *key;
    json_t *value;

    result = json_object();
    if(!result)
        return NULL;

    json_object_foreach(object, key, value)
        json_object_set_nocheck(result, key, value);

    return result;
}
