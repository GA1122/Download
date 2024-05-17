static void open_browse_if_link(GtkWidget *text_view, GtkTextIter *iter)
{
    GSList *tags = NULL, *tagp = NULL;

    tags = gtk_text_iter_get_tags (iter);
    for (tagp = tags;  tagp != NULL;  tagp = tagp->next)
    {
        GtkTextTag *tag = tagp->data;
        const char *url = g_object_get_data (G_OBJECT (tag), "url");

        if (url != 0)
        {
             
            if (getenv("KDE_FULL_SESSION") != NULL)
            {
                gint exitcode;
                gchar *arg[3];
                 
                arg[0] = (char *) "kde-open";
                arg[1] = (char *) url;
                arg[2] = NULL;

                const gboolean spawn_ret = g_spawn_sync(NULL, arg, NULL,
                                 G_SPAWN_SEARCH_PATH | G_SPAWN_STDOUT_TO_DEV_NULL,
                                 NULL, NULL, NULL, NULL, &exitcode, NULL);

                if (spawn_ret)
                    break;
            }

            GError *error = NULL;
            if (!gtk_show_uri(  NULL, url, GDK_CURRENT_TIME, &error))
                error_msg("Can't open url '%s': %s", url, error->message);

            break;
        }
    }

    if (tags)
        g_slist_free (tags);
}