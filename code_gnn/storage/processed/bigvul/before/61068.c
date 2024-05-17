get_link_name (const char *name,
               int         count,
               int         max_length)
{
    const char *format;
    char *result;
    int unshortened_length;
    gboolean use_count;

    g_assert (name != NULL);

    if (count < 0)
    {
        g_warning ("bad count in get_link_name");
        count = 0;
    }

    if (count <= 2)
    {
         
        switch (count)
        {
            default:
            {
                g_assert_not_reached ();
                 
            }

            case 0:
            {
                 
                format = "%s";
            }
            break;

            case 1:
            {
                 
                format = _("Link to %s");
            }
            break;

            case 2:
            {
                 
                format = _("Another link to %s");
            }
            break;
        }

        use_count = FALSE;
    }
    else
    {
         
        switch (count % 10)
        {
            case 1:
            {
                 
                format = _("%'dst link to %s");
            }
            break;

            case 2:
            {
                 
                format = _("%'dnd link to %s");
            }
            break;

            case 3:
            {
                 
                format = _("%'drd link to %s");
            }
            break;

            default:
            {
                 
                format = _("%'dth link to %s");
            }
            break;
        }

        use_count = TRUE;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    if (use_count)
    {
        result = g_strdup_printf (format, count, name);
    }
    else
    {
        result = g_strdup_printf (format, name);
    }

    if (max_length > 0 && (unshortened_length = strlen (result)) > max_length)
    {
        char *new_name;

        new_name = shorten_utf8_string (name, unshortened_length - max_length);
        if (new_name)
        {
            g_free (result);

            if (use_count)
            {
                result = g_strdup_printf (format, count, new_name);
            }
            else
            {
                result = g_strdup_printf (format, new_name);
            }

            g_assert (strlen (result) <= max_length);
            g_free (new_name);
        }
    }
#pragma GCC diagnostic pop
    return result;
}
