static void on_sensitive_word_selection_changed(GtkTreeSelection *sel, gpointer user_data)
{
    search_item_t *old_word = g_current_highlighted_word;
    g_current_highlighted_word = NULL;

    if (old_word && FALSE == gtk_text_buffer_get_modified(old_word->buffer))
        gtk_text_buffer_remove_tag_by_name(old_word->buffer, "current_result_bg", &(old_word->start), &(old_word->end));

    GtkTreeModel *model;
    GtkTreeIter iter;
    if (!gtk_tree_selection_get_selected(sel, &model, &iter))
        return;

    search_item_t *new_word;
    gtk_tree_model_get(model, &iter,
            SEARCH_COLUMN_ITEM, &new_word,
            -1);

    if (gtk_text_buffer_get_modified(new_word->buffer))
    {
        if (g_search_text == NULL)
            rehighlight_forbidden_words(new_word->page, new_word->tev);
        else
        {
            log_notice("searching again: '%s'", g_search_text);
            GList *searched_words = g_list_append(NULL, (gpointer)g_search_text);
            highligh_words_in_textview(new_word->page, new_word->tev, searched_words, NULL,  true);
            g_list_free(searched_words);
        }

        return;
    }

    g_current_highlighted_word = new_word;

    gtk_notebook_set_current_page(g_notebook, new_word->page);
    gtk_text_buffer_apply_tag_by_name(new_word->buffer, "current_result_bg", &(new_word->start), &(new_word->end));
    gtk_text_buffer_place_cursor(new_word->buffer, &(new_word->start));
    gtk_text_view_scroll_to_iter(new_word->tev, &(new_word->start), 0.0, false, 0, 0);
}