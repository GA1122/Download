int json_array_clear(json_t *json)
{
    json_array_t *array;
    size_t i;

    if(!json_is_array(json))
        return -1;
    array = json_to_array(json);

    for(i = 0; i < array->entries; i++)
        json_decref(array->table[i]);

    array->entries = 0;
    return 0;
}
