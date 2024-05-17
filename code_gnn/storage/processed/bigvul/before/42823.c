static void highlight_search(GtkEntry *entry)
{
    g_search_text = gtk_entry_get_text(entry);

    log_notice("searching: '%s'", g_search_text);
    GList *words = g_list_append(NULL, (gpointer)g_search_text);
    highligh_words_in_tabs(words, NULL,  true);
    g_list_free(words);
}