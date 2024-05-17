static void json_delete_object(json_object_t *object)
{
    hashtable_close(&object->hashtable);
    jsonp_free(object);
}
