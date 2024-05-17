static void process_message(GHashTable *problem_info, char *message)
{
    gchar *key, *value;

    value = strchr(message, '=');
    if (value)
    {
        key = g_ascii_strdown(message, value - message);  

        value++;
        if (key_value_ok(key, value))
        {
            if (strcmp(key, FILENAME_UID) == 0)
            {
                error_msg("Ignoring value of %s, will be determined later",
                          FILENAME_UID);
            }
            else
            {
                g_hash_table_insert(problem_info, key, xstrdup(value));
                 
                if (strcmp(key, FILENAME_TYPE) == 0)
                    g_hash_table_insert(problem_info, xstrdup(FILENAME_ANALYZER), xstrdup(value));
                 
                key = NULL;
            }
        }
        else
        {
             
            error_msg("Invalid key or value format: %s", message);
        }
        free(key);
    }
    else
    {
         
        error_msg("Invalid message format: '%s'", message);
    }
}
