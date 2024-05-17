parse_previous_duplicate_name (const char  *name,
                               char       **name_base,
                               const char **suffix,
                               int         *count)
{
    const char *tag;

    g_assert (name[0] != '\0');

    *suffix = eel_filename_get_extension_offset (name);

    if (*suffix == NULL || (*suffix)[1] == '\0')
    {
         
        *suffix = "";
    }

    tag = strstr (name, COPY_DUPLICATE_TAG);
    if (tag != NULL)
    {
        if (tag > *suffix)
        {
             
            *suffix = "";
        }
        *name_base = extract_string_until (name, tag);
        *count = 1;
        return;
    }


    tag = strstr (name, ANOTHER_COPY_DUPLICATE_TAG);
    if (tag != NULL)
    {
        if (tag > *suffix)
        {
             
            *suffix = "";
        }
        *name_base = extract_string_until (name, tag);
        *count = 2;
        return;
    }


     
    tag = strstr (name, X11TH_COPY_DUPLICATE_TAG);

    if (tag == NULL)
    {
        tag = strstr (name, X12TH_COPY_DUPLICATE_TAG);
    }
    if (tag == NULL)
    {
        tag = strstr (name, X13TH_COPY_DUPLICATE_TAG);
    }

    if (tag == NULL)
    {
        tag = strstr (name, ST_COPY_DUPLICATE_TAG);
    }
    if (tag == NULL)
    {
        tag = strstr (name, ND_COPY_DUPLICATE_TAG);
    }
    if (tag == NULL)
    {
        tag = strstr (name, RD_COPY_DUPLICATE_TAG);
    }
    if (tag == NULL)
    {
        tag = strstr (name, TH_COPY_DUPLICATE_TAG);
    }

     
    if (tag != NULL)
    {
         
        tag = strstr (name, _(" ("));
        if (tag != NULL)
        {
            if (tag > *suffix)
            {
                 
                *suffix = "";
            }
            *name_base = extract_string_until (name, tag);
             
            if (sscanf (tag, _(" (%'d"), count) == 1)
            {
                if (*count < 1 || *count > 1000000)
                {
                     
                    *count = 0;
                }
                return;
            }
            *count = 0;
            return;
        }
    }


    *count = 0;
    if (**suffix != '\0')
    {
        *name_base = extract_string_until (name, *suffix);
    }
    else
    {
        *name_base = g_strdup (name);
    }
}
