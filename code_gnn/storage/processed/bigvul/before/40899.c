static void json_delete_string(json_string_t *string)
{
    jsonp_free(string->value);
    jsonp_free(string);
}
