static void set_auto_event_chain(GtkButton *button, gpointer user_data)
{
    g_expert_mode = false;

    workflow_t *w = (workflow_t *)user_data;
    config_item_info_t *info = workflow_get_config_info(w);
    log_notice("selected workflow '%s'", ci_get_screen_name(info));

    GList *wf_event_list = wf_get_event_list(w);
    while(wf_event_list)
    {
        g_auto_event_list = g_list_append(g_auto_event_list, xstrdup(ec_get_name(wf_event_list->data)));
        load_single_event_config_data_from_user_storage((event_config_t *)wf_event_list->data);

        wf_event_list = g_list_next(wf_event_list);
    }

    gint current_page_no = gtk_notebook_get_current_page(g_assistant);
    gint next_page_no = select_next_page_no(current_page_no, NULL);

     
    if (current_page_no == next_page_no)
        on_page_prepare(g_assistant, gtk_notebook_get_nth_page(g_assistant, next_page_no), NULL);
    else
        gtk_notebook_set_current_page(g_assistant, next_page_no);

     
    show_next_step_button();
}