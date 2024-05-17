static void append_item_to_ls_details(gpointer name, gpointer value, gpointer data)
{
    problem_item *item = (problem_item*)value;
    struct cd_stats *stats = data;
    GtkTreeIter iter;

    gtk_list_store_append(g_ls_details, &iter);
    stats->filecount++;

    if (item->flags & CD_FLAG_TXT)
    {
        if (item->flags & CD_FLAG_ISEDITABLE && strcmp(name, FILENAME_ANACONDA_TB) != 0)
        {
            GtkWidget *tab_lbl = gtk_label_new((char *)name);
            GtkWidget *tev = gtk_text_view_new();

            if (strcmp(name, FILENAME_COMMENT) == 0 || strcmp(name, FILENAME_REASON) == 0)
                gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tev), GTK_WRAP_WORD);

            gtk_widget_override_font(GTK_WIDGET(tev), g_monospace_font);
            load_text_to_text_view(GTK_TEXT_VIEW(tev), (char *)name);
             
            GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tev));
             
            gtk_text_buffer_create_tag(buf, "search_result_bg", "background", "red", NULL);
            gtk_text_buffer_create_tag(buf, "current_result_bg", "background", "green", NULL);
            GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
            gtk_container_add(GTK_CONTAINER(sw), tev);
            gtk_notebook_append_page(g_notebook, sw, tab_lbl);
        }
        stats->filesize += strlen(item->content);
         
        if (!strchr(item->content, '\n'))
        {
            gtk_list_store_set(g_ls_details, &iter,
                              DETAIL_COLUMN_NAME, (char *)name,
                              DETAIL_COLUMN_VALUE, item->content,
                              -1);
        }
        else
        {
            gtk_list_store_set(g_ls_details, &iter,
                              DETAIL_COLUMN_NAME, (char *)name,
                              DETAIL_COLUMN_VALUE, _("(click here to view/edit)"),
                              -1);
        }
    }
    else if (item->flags & CD_FLAG_BIN)
    {
        struct stat statbuf;
        statbuf.st_size = 0;
        if (stat(item->content, &statbuf) == 0)
        {
            stats->filesize += statbuf.st_size;
            char *msg = xasprintf(_("(binary file, %llu bytes)"), (long long)statbuf.st_size);
            gtk_list_store_set(g_ls_details, &iter,
                                  DETAIL_COLUMN_NAME, (char *)name,
                                  DETAIL_COLUMN_VALUE, msg,
                                  -1);
            free(msg);
        }
    }

    int cur_value;
    if (item->selected_by_user == 0)
        cur_value = item->default_by_reporter;
    else
        cur_value = !!(item->selected_by_user + 1);  

    gtk_list_store_set(g_ls_details, &iter,
            DETAIL_COLUMN_CHECKBOX, cur_value,
            -1);
}