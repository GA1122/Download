json_t *json_object_iter_value(void *iter)
{
    if(!iter)
        return NULL;

    return (json_t *)hashtable_iter_value(iter);
}
