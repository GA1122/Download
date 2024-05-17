void *json_object_key_to_iter(const char *key)
{
    if(!key)
        return NULL;

    return hashtable_key_to_iter(key);
}
