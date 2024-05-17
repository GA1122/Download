static bool highligh_words_in_textview(int page, GtkTextView *tev, GList *words, GList *ignored_words, bool case_insensitive)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tev);
    gtk_text_buffer_set_modified(buffer, FALSE);

    GtkWidget *notebook_child = gtk_notebook_get_nth_page(g_notebook, page);
    GtkWidget *tab_lbl = gtk_notebook_get_tab_label(g_notebook, notebook_child);

     
    bool buffer_removing = false;

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(g_ls_sensitive_list), &iter);

     
    g_signal_handler_block(g_tv_sensitive_sel, g_tv_sensitive_sel_hndlr);

    while (valid)
    {
        char *text = NULL;
        search_item_t *word = NULL;

        gtk_tree_model_get(GTK_TREE_MODEL(g_ls_sensitive_list), &iter,
                SEARCH_COLUMN_TEXT, &text,
                SEARCH_COLUMN_ITEM, &word,
                -1);

        free(text);

        if (word->buffer == buffer)
        {
            buffer_removing = true;

            valid = gtk_list_store_remove(g_ls_sensitive_list, &iter);

            if (word == g_current_highlighted_word)
                g_current_highlighted_word = NULL;

            free(word);
        }
        else
        {
            if(buffer_removing)
                break;

            valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(g_ls_sensitive_list), &iter);
        }
    }

     
    g_signal_handler_unblock(g_tv_sensitive_sel, g_tv_sensitive_sel_hndlr);

    GtkTextIter start_find;
    gtk_text_buffer_get_start_iter(buffer, &start_find);
    GtkTextIter end_find;
    gtk_text_buffer_get_end_iter(buffer, &end_find);

    gtk_text_buffer_remove_tag_by_name(buffer, "search_result_bg", &start_find, &end_find);
    gtk_text_buffer_remove_tag_by_name(buffer, "current_result_bg", &start_find, &end_find);

    PangoAttrList *attrs = gtk_label_get_attributes(GTK_LABEL(tab_lbl));
    gtk_label_set_attributes(GTK_LABEL(tab_lbl), NULL);
    pango_attr_list_unref(attrs);

    GList *result = NULL;
    GList *ignored_words_in_buffer = NULL;

    ignored_words_in_buffer = find_words_in_text_buffer(page,
                                                        tev,
                                                        ignored_words,
                                                        NULL,
                                                        start_find,
                                                        end_find,
                                                         false);


    result = find_words_in_text_buffer(page,
                                       tev,
                                       words,
                                       ignored_words_in_buffer,
                                       start_find,
                                       end_find,
                                       case_insensitive
                                        );

    for (GList *w = result; w; w = g_list_next(w))
    {
        search_item_t *item = (search_item_t *)w->data;
        gtk_text_buffer_apply_tag_by_name(buffer, "search_result_bg",
                                          sitem_get_start_iter(item),
                                          sitem_get_end_iter(item));
    }

    if (result)
    {
        PangoAttrList *attrs = pango_attr_list_new();
        PangoAttribute *foreground_attr = pango_attr_foreground_new(65535, 0, 0);
        pango_attr_list_insert(attrs, foreground_attr);
        PangoAttribute *underline_attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
        pango_attr_list_insert(attrs, underline_attr);
        gtk_label_set_attributes(GTK_LABEL(tab_lbl), attrs);

         
        result = g_list_sort(result, (GCompareFunc)sitem_compare);

        GList *search_result = result;
        for ( ; search_result != NULL; search_result = g_list_next(search_result))
        {
            search_item_t *word = (search_item_t *)search_result->data;

            const gchar *file_name = gtk_label_get_text(GTK_LABEL(tab_lbl));

             
            GtkTreeIter new_row;
            if (valid)
                 
                 
                 
                gtk_list_store_insert_before(g_ls_sensitive_list, &new_row, &iter);
            else
                 
                 
                gtk_list_store_append(g_ls_sensitive_list, &new_row);

             
            search_item_to_list_store_item(g_ls_sensitive_list, &new_row, file_name, word);
        }
    }

    g_list_free_full(ignored_words_in_buffer, free);
    g_list_free(result);

    return result != NULL;
}
