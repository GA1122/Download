static int json_object_equal(json_t *object1, json_t *object2)
{
    const char *key;
    json_t *value1, *value2;

    if(json_object_size(object1) != json_object_size(object2))
        return 0;

    json_object_foreach(object1, key, value1) {
        value2 = json_object_get(object2, key);

        if(!json_equal(value1, value2))
            return 0;
    }

    return 1;
}
