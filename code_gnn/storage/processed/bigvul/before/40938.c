json_t *json_true(void)
{
    static json_t the_true = {JSON_TRUE, (size_t)-1};
    return &the_true;
}
