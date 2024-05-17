static void add_pages(void)
{
    g_builder = make_builder();

    int i;
    int page_no = 0;
    for (i = 0; page_names[i] != NULL; i++)
    {
        GtkWidget *page = GTK_WIDGET(gtk_builder_get_object(g_builder, page_names[i]));
        pages[i].page_widget = page;
        pages[i].page_no = page_no++;
        gtk_notebook_append_page(g_assistant, page, gtk_label_new(pages[i].title));
        log_notice("added page: %s", page_names[i]);
    }

     
    g_lbl_cd_reason        = GTK_LABEL(        gtk_builder_get_object(g_builder, "lbl_cd_reason"));
    g_box_events           = GTK_BOX(          gtk_builder_get_object(g_builder, "vb_events"));
    g_box_workflows        = GTK_BOX(          gtk_builder_get_object(g_builder, "vb_workflows"));
    g_lbl_event_log        = GTK_LABEL(        gtk_builder_get_object(g_builder, "lbl_event_log"));
    g_tv_event_log         = GTK_TEXT_VIEW(    gtk_builder_get_object(g_builder, "tv_event_log"));
    g_tv_comment           = GTK_TEXT_VIEW(    gtk_builder_get_object(g_builder, "tv_comment"));
    g_eb_comment           = GTK_EVENT_BOX(    gtk_builder_get_object(g_builder, "eb_comment"));
    g_cb_no_comment        = GTK_CHECK_BUTTON( gtk_builder_get_object(g_builder, "cb_no_comment"));
    g_tv_details           = GTK_TREE_VIEW(    gtk_builder_get_object(g_builder, "tv_details"));
    g_tb_approve_bt        = GTK_TOGGLE_BUTTON(gtk_builder_get_object(g_builder, "cb_approve_bt"));
    g_search_entry_bt      = GTK_ENTRY(        gtk_builder_get_object(g_builder, "entry_search_bt"));
    g_container_details1   = GTK_CONTAINER(    gtk_builder_get_object(g_builder, "container_details1"));
    g_container_details2   = GTK_CONTAINER(    gtk_builder_get_object(g_builder, "container_details2"));
    g_btn_add_file         = GTK_BUTTON(       gtk_builder_get_object(g_builder, "btn_add_file"));
    g_lbl_size             = GTK_LABEL(        gtk_builder_get_object(g_builder, "lbl_size"));
    g_notebook             = GTK_NOTEBOOK(     gtk_builder_get_object(g_builder, "notebook_edit"));
    g_ls_sensitive_list    = GTK_LIST_STORE(   gtk_builder_get_object(g_builder, "ls_sensitive_words"));
    g_tv_sensitive_list    = GTK_TREE_VIEW(    gtk_builder_get_object(g_builder, "tv_sensitive_words"));
    g_tv_sensitive_sel     = GTK_TREE_SELECTION( gtk_builder_get_object(g_builder, "tv_sensitive_words_selection"));
    g_rb_forbidden_words   = GTK_RADIO_BUTTON( gtk_builder_get_object(g_builder, "rb_forbidden_words"));
    g_rb_custom_search     = GTK_RADIO_BUTTON( gtk_builder_get_object(g_builder, "rb_custom_search"));
    g_exp_search           = GTK_EXPANDER(     gtk_builder_get_object(g_builder, "expander_search"));
    g_spinner_event_log    = GTK_SPINNER(      gtk_builder_get_object(g_builder, "spinner_event_log"));
    g_img_process_fail     = GTK_IMAGE(      gtk_builder_get_object(g_builder, "img_process_fail"));
    g_btn_startcast        = GTK_BUTTON(    gtk_builder_get_object(g_builder, "btn_startcast"));
    g_exp_report_log       = GTK_EXPANDER(     gtk_builder_get_object(g_builder, "expand_report"));

    gtk_widget_set_no_show_all(GTK_WIDGET(g_spinner_event_log), true);

    gtk_widget_override_font(GTK_WIDGET(g_tv_event_log), g_monospace_font);
    fix_all_wrapped_labels(GTK_WIDGET(g_assistant));

    g_signal_connect(g_cb_no_comment, "toggled", G_CALLBACK(on_no_comment_toggled), NULL);

    g_signal_connect(g_rb_forbidden_words, "toggled", G_CALLBACK(on_forbidden_words_toggled), NULL);
    g_signal_connect(g_rb_custom_search, "toggled", G_CALLBACK(on_custom_search_toggled), NULL);

     
    GdkRGBA color;
    gdk_rgba_parse(&color, "#CC3333");
    gtk_widget_override_color(GTK_WIDGET(g_eb_comment), GTK_STATE_FLAG_NORMAL, &color);

    g_signal_connect(g_tv_details, "key-press-event", G_CALLBACK(on_key_press_event_in_item_list), NULL);
    g_tv_sensitive_sel_hndlr = g_signal_connect(g_tv_sensitive_sel, "changed", G_CALLBACK(on_sensitive_word_selection_changed), NULL);


}