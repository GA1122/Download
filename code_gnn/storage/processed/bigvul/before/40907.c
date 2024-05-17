json_t *json_null(void)
{
    static json_t the_null = {JSON_NULL, (size_t)-1};
    return &the_null;
}
