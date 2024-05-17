static json_t *json_real_copy(const json_t *real)
{
    return json_real(json_real_value(real));
}
