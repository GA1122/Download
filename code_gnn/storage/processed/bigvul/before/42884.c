void update_gui_state_from_problem_data(int flags)
{
    update_window_title();
    remove_tabs_from_notebook(g_notebook);

    const char *reason = problem_data_get_content_or_NULL(g_cd, FILENAME_REASON);
    const char *not_reportable = problem_data_get_content_or_NULL(g_cd,
                                                                  FILENAME_NOT_REPORTABLE);

    char *t = xasprintf("%s%s%s",
                        not_reportable ? : "",
                        not_reportable ? " " : "",
                        reason ? : _("(no description)"));

    gtk_label_set_text(g_lbl_cd_reason, t);
    free(t);

    gtk_list_store_clear(g_ls_details);
    struct cd_stats stats = { 0 };
    g_hash_table_foreach(g_cd, append_item_to_ls_details, &stats);
    char *msg = xasprintf(_("%llu bytes, %u files"), (long long)stats.filesize, stats.filecount);
    gtk_label_set_text(g_lbl_size, msg);
    free(msg);

    load_text_to_text_view(g_tv_comment, FILENAME_COMMENT);

    add_workflow_buttons(g_box_workflows, g_workflow_list,
                        G_CALLBACK(set_auto_event_chain));

     
    event_gui_data_t *active_button = NULL;
    if (g_expert_mode == true)
    {
        gtk_widget_show(GTK_WIDGET(g_box_events));
        active_button = add_event_buttons(
                    g_box_events,
                    &g_list_events,
                    g_events,
                    G_CALLBACK(event_rb_was_toggled)
        );
    }

    if (flags & UPDATE_SELECTED_EVENT && g_expert_mode)
    {
         
        free(g_event_selected);
        g_event_selected = NULL;
        if (active_button)
        {
            g_event_selected = xstrdup(active_button->event_name);
        }
        log_info("g_event_selected='%s'", g_event_selected);
    }
     
    gtk_widget_show_all(GTK_WIDGET(g_wnd_assistant));
}