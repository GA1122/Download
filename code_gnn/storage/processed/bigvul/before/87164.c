static cJSON_bool print_number(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    double d = item->valuedouble;
    int length = 0;
    size_t i = 0;
    unsigned char number_buffer[26];  
    unsigned char decimal_point = get_decimal_point();
    double test;

    if (output_buffer == NULL)
    {
        return false;
    }

     
    if ((d * 0) != 0)
    {
        length = sprintf((char*)number_buffer, "null");
    }
    else
    {
         
        length = sprintf((char*)number_buffer, "%1.15g", d);

         
        if ((sscanf((char*)number_buffer, "%lg", &test) != 1) || ((double)test != d))
        {
             
            length = sprintf((char*)number_buffer, "%1.17g", d);
        }
    }

     
    if ((length < 0) || (length > (int)(sizeof(number_buffer) - 1)))
    {
        return false;
    }

     
    output_pointer = ensure(output_buffer, (size_t)length + sizeof(""));
    if (output_pointer == NULL)
    {
        return false;
    }

     
    for (i = 0; i < ((size_t)length); i++)
    {
        if (number_buffer[i] == decimal_point)
        {
            output_pointer[i] = '.';
            continue;
        }

        output_pointer[i] = number_buffer[i];
    }
    output_pointer[i] = '\0';

    output_buffer->offset += (size_t)length;

    return true;
}
