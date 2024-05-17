static void remove_tabs_from_notebook(GtkNotebook *notebook)
{
    gint n_pages = gtk_notebook_get_n_pages(notebook);
    int ii;

    for (ii = 0; ii < n_pages; ii++)
    {
         
        gtk_notebook_remove_page(notebook, 0);  
    }

     
    g_signal_handler_block(g_tv_sensitive_sel, g_tv_sensitive_sel_hndlr);

    g_current_highlighted_word = NULL;

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(g_ls_sensitive_list), &iter);
    while (valid)
    {
        char *text = NULL;
        search_item_t *word = NULL;

        gtk_tree_model_get(GTK_TREE_MODEL(g_ls_sensitive_list), &iter,
                SEARCH_COLUMN_TEXT, &text,
                SEARCH_COLUMN_ITEM, &word,
                -1);

        free(text);
        free(word);

        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(g_ls_sensitive_list), &iter);
    }
    gtk_list_store_clear(g_ls_sensitive_list);
    g_signal_handler_unblock(g_tv_sensitive_sel, g_tv_sensitive_sel_hndlr);
}