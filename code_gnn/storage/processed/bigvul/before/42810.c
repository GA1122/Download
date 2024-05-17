static GList *find_words_in_text_buffer(int page,
                                        GtkTextView *tev,
                                        GList *words,
                                        GList *ignore_sitem_list,
                                        GtkTextIter start_find,
                                        GtkTextIter end_find,
                                        bool case_insensitive
                                        )
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tev);
    gtk_text_buffer_set_modified(buffer, FALSE);

    GList *found_words = NULL;
    GtkTextIter start_match;
    GtkTextIter end_match;

    for (GList *w = words; w; w = g_list_next(w))
    {
        gtk_text_buffer_get_start_iter(buffer, &start_find);

        const char *search_word = w->data;
        while (search_word && search_word[0] && gtk_text_iter_forward_search(&start_find, search_word,
                    GTK_TEXT_SEARCH_TEXT_ONLY | (case_insensitive ? GTK_TEXT_SEARCH_CASE_INSENSITIVE : 0),
                    &start_match,
                    &end_match, NULL))
        {
            search_item_t *found_word = sitem_new(
                    page,
                    buffer,
                    tev,
                    start_match,
                    end_match
                );
            int offset = gtk_text_iter_get_offset(&end_match);
            gtk_text_buffer_get_iter_at_offset(buffer, &start_find, offset);

            if (sitem_is_in_sitemlist(found_word, ignore_sitem_list))
            {
                sitem_free(found_word);
                continue;
            }

            found_words = g_list_prepend(found_words, found_word);
        }
    }

    return found_words;
}