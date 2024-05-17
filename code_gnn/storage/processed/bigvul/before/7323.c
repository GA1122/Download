string_match (const char *string, const char *mask, int case_sensitive)
{
    char last, *mask2;
    int len_string, len_mask, rc;

    if (!mask || !mask[0])
        return 0;

     
    if (strcmp (mask, "*") == 0)
        return 1;

    len_string = strlen (string);
    len_mask = strlen (mask);

    last = mask[len_mask - 1];

     
    if ((mask[0] == '*') && (last != '*'))
    {
         
        if (len_string < len_mask - 1)
            return 0;
         
        if ((case_sensitive && (strcmp (string + len_string - (len_mask - 1),
                                        mask + 1) == 0))
            || (!case_sensitive && (string_strcasecmp (string + len_string - (len_mask - 1),
                                                       mask + 1) == 0)))
            return 1;
         
        return 0;
    }

     
    if ((mask[0] != '*') && (last == '*'))
    {
         
        if (len_string < len_mask - 1)
            return 0;
         
        if ((case_sensitive && (strncmp (string, mask, len_mask - 1) == 0))
            || (!case_sensitive && (string_strncasecmp (string,
                                                        mask,
                                                        len_mask - 1) == 0)))
            return 1;
         
        return 0;
    }

     
    if ((mask[0] == '*') && (last == '*'))
    {
         
        if (len_string < len_mask - 1)
            return 0;
         
        mask2 = string_strndup (mask + 1, len_mask - 2);
        if (!mask2)
            return 0;
         
        rc = ((case_sensitive && strstr (string, mask2))
              || (!case_sensitive && string_strcasestr (string, mask2))) ?
            1 : 0;
         
        free (mask2);
        return rc;
    }

     
    if ((case_sensitive && (strcmp (string, mask) == 0))
        || (!case_sensitive && (string_strcasecmp (string, mask) == 0)))
        return 1;

     
    return 0;
}
