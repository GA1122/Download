static void append_to_textview(GtkTextView *tv, const char *str)
{
    GtkTextBuffer *tb = gtk_text_view_get_buffer(tv);

     
    GtkTextIter text_iter;
    gtk_text_buffer_get_end_iter(tb, &text_iter);
    gtk_text_buffer_place_cursor(tb, &text_iter);

     
    const gchar *end;
    while (!g_utf8_validate(str, -1, &end))
    {
        gtk_text_buffer_insert_at_cursor(tb, str, end - str);
        char buf[8];
        unsigned len = snprintf(buf, sizeof(buf), "<%02X>", (unsigned char)*end);
        gtk_text_buffer_insert_at_cursor(tb, buf, len);
        str = end + 1;
    }

    gtk_text_buffer_get_end_iter(tb, &text_iter);

    const char *last = str;
    GList *urls = find_url_tokens(str);
    for (GList *u = urls; u; u = g_list_next(u))
    {
        const struct url_token *const t = (struct url_token *)u->data;
        if (last < t->start)
            gtk_text_buffer_insert(tb, &text_iter, last, t->start - last);

        GtkTextTag *tag;
        tag = gtk_text_buffer_create_tag (tb, NULL, "foreground", "blue",
                                          "underline", PANGO_UNDERLINE_SINGLE, NULL);
        char *url = xstrndup(t->start, t->len);
        g_object_set_data (G_OBJECT (tag), "url", url);

        gtk_text_buffer_insert_with_tags(tb, &text_iter, url, -1, tag, NULL);

        last = t->start + t->len;
    }

    g_list_free_full(urls, g_free);

    if (last[0] != '\0')
        gtk_text_buffer_insert(tb, &text_iter, last, strlen(last));

     
    gtk_text_view_scroll_to_iter(tv, &text_iter,
                  0.0,   FALSE,   0,   0);
}