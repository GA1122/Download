static void save_items_from_notepad(void)
{
    gint n_pages = gtk_notebook_get_n_pages(g_notebook);
    int i = 0;

    GtkWidget *notebook_child;
    GtkTextView *tev;
    GtkWidget *tab_lbl;
    const char *item_name;

    for (i = 0; i < n_pages; i++)
    {
        notebook_child = gtk_notebook_get_nth_page(g_notebook, i);
        tev = GTK_TEXT_VIEW(gtk_bin_get_child(GTK_BIN(notebook_child)));
        tab_lbl = gtk_notebook_get_tab_label(g_notebook, notebook_child);
        item_name = gtk_label_get_text(GTK_LABEL(tab_lbl));
        log_notice("saving: '%s'", item_name);

        save_text_from_text_view(tev, item_name);
    }
}