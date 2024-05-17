static void load_text_to_text_view(GtkTextView *tv, const char *name)
{
     
     
     
    g_hash_table_insert(g_loaded_texts, (gpointer)xstrdup(name), (gpointer)1);

    const char *str = g_cd ? problem_data_get_content_or_NULL(g_cd, name) : NULL;
     
     
     

    reload_text_to_text_view(tv, str);
}
