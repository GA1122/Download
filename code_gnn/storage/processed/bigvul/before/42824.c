static gboolean highlight_search_on_timeout(gpointer user_data)
{
    g_timeout = 0;
    highlight_search(GTK_ENTRY(user_data));
     
    return false;
}