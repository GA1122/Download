static void start_event_run(const char *event_name)
{
     

     
    correct_bz_private_goup_name(event_name);

    struct run_event_state *state = new_run_event_state();
    state->logging_callback = run_event_gtk_logging;
    state->error_callback = run_event_gtk_error;
    state->alert_callback = run_event_gtk_alert;
    state->ask_callback = run_event_gtk_ask;
    state->ask_yes_no_callback = run_event_gtk_ask_yes_no;
    state->ask_yes_no_yesforever_callback = run_event_gtk_ask_yes_no_yesforever;
    state->ask_yes_no_save_result_callback = run_event_gtk_ask_yes_no_save_result;
    state->ask_password_callback = run_event_gtk_ask_password;

    if (prepare_commands(state, g_dump_dir_name, event_name) == 0)
    {
 no_cmds:
         
        free_run_event_state(state);
        char *msg = xasprintf(_("No processing for event '%s' is defined"), event_name);
        append_to_textview(g_tv_event_log, msg);
        free(msg);
        cancel_processing(g_lbl_event_log, _("Processing failed."), TERMINATE_NOFLAGS);
        return;
    }

    struct dump_dir *dd = wizard_open_directory_for_writing(g_dump_dir_name);
    dd_close(dd);
    if (!dd)
    {
        free_run_event_state(state);
        if (!g_expert_mode)
        {
            cancel_processing(g_lbl_event_log, _("Processing interrupted: can't continue without writable directory."), TERMINATE_NOFLAGS);
        }
        return;  
    }

    set_excluded_envvar();
    GList *env_list = export_event_config(event_name);

    if (spawn_next_command(state, g_dump_dir_name, event_name, EXECFLG_SETPGID) < 0)
    {
        unexport_event_config(env_list);
        goto no_cmds;
    }
    g_event_child_pid = state->command_pid;

     
    struct analyze_event_data *evd = xzalloc(sizeof(*evd));
    evd->run_state = state;
    evd->event_name = xstrdup(event_name);
    evd->env_list = env_list;
    evd->event_log = strbuf_new();
    evd->fd = state->command_out_fd;

    state->logging_param = evd;
    state->error_param = evd;
    state->interaction_param = evd;

    ndelay_on(evd->fd);
    evd->channel = g_io_channel_unix_new(evd->fd);
    g_event_source_id = g_io_add_watch(evd->channel,
            G_IO_IN | G_IO_ERR | G_IO_HUP,  
            consume_cmd_output,
            evd
    );

    gtk_label_set_text(g_lbl_event_log, _("Processing..."));
    log_notice("running event '%s' on '%s'", event_name, g_dump_dir_name);
    char *msg = xasprintf("--- Running %s ---\n", event_name);
    append_to_textview(g_tv_event_log, msg);
    free(msg);

     
    gtk_widget_hide(GTK_WIDGET(g_img_process_fail));

    gtk_widget_show(GTK_WIDGET(g_spinner_event_log));
    gtk_widget_show(g_btn_stop);
     
    gtk_widget_set_sensitive(g_btn_close, false);
    gtk_widget_set_sensitive(g_btn_next, false);
}