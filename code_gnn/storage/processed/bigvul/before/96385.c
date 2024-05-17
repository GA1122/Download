static gboolean key_value_ok(gchar *key, gchar *value)
{
    char *i;

     
    for (i = key; *i != 0; i++)
    {
        if (!isalpha(*i) && (*i != '-') && (*i != '_') && (*i != ' '))
            return FALSE;
    }

     
    if (strcmp(key, "basename") == 0
     || strcmp(key, FILENAME_TYPE) == 0
    )
    {
        if (!is_correct_filename(value))
        {
            error_msg("Value of '%s' ('%s') is not a valid directory name",
                      key, value);
            return FALSE;
        }
    }

    return TRUE;
}
