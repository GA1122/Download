void json_delete(json_t *json)
{
    if(json_is_object(json))
        json_delete_object(json_to_object(json));

    else if(json_is_array(json))
        json_delete_array(json_to_array(json));

    else if(json_is_string(json))
        json_delete_string(json_to_string(json));

    else if(json_is_integer(json))
        json_delete_integer(json_to_integer(json));

    else if(json_is_real(json))
        json_delete_real(json_to_real(json));

     
}
