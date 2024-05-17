json_t *json_array(void)
{
    json_array_t *array = jsonp_malloc(sizeof(json_array_t));
    if(!array)
        return NULL;
    json_init(&array->json, JSON_ARRAY);

    array->entries = 0;
    array->size = 8;

    array->table = jsonp_malloc(array->size * sizeof(json_t *));
    if(!array->table) {
        jsonp_free(array);
        return NULL;
    }

    array->visited = 0;

    return &array->json;
}
