int json_object_iter_set_new(json_t *json, void *iter, json_t *value)
{
    if(!json_is_object(json) || !iter || !value)
        return -1;

    hashtable_iter_set(iter, value);
    return 0;
}
