static gboolean highligh_words_in_tabs(GList *forbidden_words,  GList *allowed_words, bool case_insensitive)
{
    gboolean found = false;

    gint n_pages = gtk_notebook_get_n_pages(g_notebook);
    int page = 0;
    for (page = 0; page < n_pages; page++)
    {
        GtkWidget *notebook_child = gtk_notebook_get_nth_page(g_notebook, page);
        GtkWidget *tab_lbl = gtk_notebook_get_tab_label(g_notebook, notebook_child);

        const char *const lbl_txt = gtk_label_get_text(GTK_LABEL(tab_lbl));
        if (strncmp(lbl_txt, "page 1", 5) == 0 || strcmp(FILENAME_COMMENT, lbl_txt) == 0)
            continue;

        GtkTextView *tev = GTK_TEXT_VIEW(gtk_bin_get_child(GTK_BIN(notebook_child)));
        found |= highligh_words_in_textview(page, tev, forbidden_words, allowed_words, case_insensitive);
    }

    GtkTreeIter iter;
    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(g_ls_sensitive_list), &iter))
        gtk_tree_selection_select_iter(g_tv_sensitive_sel, &iter);

    return found;
}