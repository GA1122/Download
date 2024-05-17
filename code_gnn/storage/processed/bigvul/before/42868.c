static void search_item_to_list_store_item(GtkListStore *store, GtkTreeIter *new_row,
        const gchar *file_name, search_item_t *word)
{
    GtkTextIter *beg = gtk_text_iter_copy(&(word->start));
    gtk_text_iter_backward_line(beg);

    GtkTextIter *end = gtk_text_iter_copy(&(word->end));
     
    if (gtk_text_iter_forward_line(end))
    {
         
        gtk_text_iter_forward_line(end);

         
        gtk_text_iter_backward_char(end);
    }

    gchar *tmp = gtk_text_buffer_get_text(word->buffer, beg, &(word->start),
             FALSE);
    gchar *prefix = g_markup_escape_text(tmp,  -1);
    g_free(tmp);

    tmp = gtk_text_buffer_get_text(word->buffer, &(word->start), &(word->end),
             FALSE);
    gchar *text = g_markup_escape_text(tmp,  -1);
    g_free(tmp);

    tmp = gtk_text_buffer_get_text(word->buffer, &(word->end), end,
             FALSE);
    gchar *suffix = g_markup_escape_text(tmp,  -1);
    g_free(tmp);

    char *content = xasprintf("%s<span foreground=\"red\">%s</span>%s", prefix, text, suffix);

    g_free(suffix);
    g_free(text);
    g_free(prefix);

    gtk_text_iter_free(end);
    gtk_text_iter_free(beg);

    gtk_list_store_set(store, new_row,
            SEARCH_COLUMN_FILE, file_name,
            SEARCH_COLUMN_TEXT, content,
            SEARCH_COLUMN_ITEM, word,
            -1);
}
