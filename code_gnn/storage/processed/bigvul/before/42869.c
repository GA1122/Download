static void search_timeout(GtkEntry *entry)
{
     
    if (g_timeout != 0)
        g_source_remove(g_timeout);
    g_timeout = g_timeout_add(500, &highlight_search_on_timeout, (gpointer)entry);
}
