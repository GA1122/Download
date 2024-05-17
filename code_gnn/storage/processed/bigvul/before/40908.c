double json_number_value(const json_t *json)
{
    if(json_is_integer(json))
        return (double)json_integer_value(json);
    else if(json_is_real(json))
        return json_real_value(json);
    else
        return 0.0;
}
