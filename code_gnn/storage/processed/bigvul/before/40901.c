json_t *json_false(void)
{
    static json_t the_false = {JSON_FALSE, (size_t)-1};
    return &the_false;
}
