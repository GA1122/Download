int json_object_set_new_nocheck(json_t *json, const char *key, json_t *value)
{
    json_object_t *object;

    if(!value)
        return -1;

    if(!key || !json_is_object(json) || json == value)
    {
        json_decref(value);
        return -1;
    }
    object = json_to_object(json);

    if(hashtable_set(&object->hashtable, key, object->serial++, value))
    {
        json_decref(value);
        return -1;
    }

    return 0;
}
