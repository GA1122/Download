static void assistant_quit_cb(void *obj, void *data)
{
     
    if (g_event_source_id != 0)
    {
        g_source_remove(g_event_source_id);
        g_event_source_id = 0;
    }

    cancel_event_run();

    if (g_loaded_texts)
    {
        g_hash_table_destroy(g_loaded_texts);
        g_loaded_texts = NULL;
    }

    gtk_widget_destroy(GTK_WIDGET(g_wnd_assistant));
    g_wnd_assistant = (void *)0xdeadbeaf;
}