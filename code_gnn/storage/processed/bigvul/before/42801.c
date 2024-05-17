static gboolean consume_cmd_output(GIOChannel *source, GIOCondition condition, gpointer data)
{
    struct analyze_event_data *evd = data;
    struct run_event_state *run_state = evd->run_state;

    bool stop_requested = false;
    int retval = consume_event_command_output(run_state, g_dump_dir_name);

    if (retval < 0 && errno == EAGAIN)
         
        return TRUE;  

     

    if (WIFEXITED(run_state->process_status)
     && WEXITSTATUS(run_state->process_status) == EXIT_STOP_EVENT_RUN
    ) {
        retval = 0;
        run_state->process_status = 0;
        stop_requested = true;
        terminate_event_chain(TERMINATE_NOFLAGS);
    }

    unexport_event_config(evd->env_list);
    evd->env_list = NULL;

     
    g_event_child_pid = -1;
    evd->run_state->command_pid = -1;  

     
    if (evd->event_log->len != 0 && evd->event_log->buf[evd->event_log->len - 1] != '\n')
        save_to_event_log(evd, "\n");

     
    if (retval != 0 || evd->event_log_state == LOGSTATE_FIRSTLINE)
    {
        char *msg = exit_status_as_string(evd->event_name, run_state->process_status);
        if (retval != 0)
        {
             
            evd->event_log_state = LOGSTATE_ERRLINE;
        }
        append_to_textview(g_tv_event_log, msg);
        save_to_event_log(evd, msg);
        free(msg);
    }

     
    update_event_log_on_disk(evd->event_log->buf);
    strbuf_clear(evd->event_log);
    evd->event_log_state = LOGSTATE_FIRSTLINE;

    struct dump_dir *dd = NULL;
    if (geteuid() == 0)
    {
         
        dd = dd_opendir(g_dump_dir_name, 0);
        if (dd)
            dd_sanitize_mode_and_owner(dd);
    }

    if (retval == 0 && !g_expert_mode)
    {
         
        if (!dd)  
            dd = dd_opendir(g_dump_dir_name, DD_OPEN_READONLY | DD_FAIL_QUIETLY_EACCES);
        if (!dd)
            xfunc_die();
        char *not_reportable = dd_load_text_ext(dd, FILENAME_NOT_REPORTABLE, 0
                                            | DD_LOAD_TEXT_RETURN_NULL_ON_FAILURE
                                            | DD_FAIL_QUIETLY_ENOENT
                                            | DD_FAIL_QUIETLY_EACCES);
        if (not_reportable)
            retval = 256;
        free(not_reportable);
    }
    if (dd)
        dd_close(dd);

     
    if (stop_requested
     || retval != 0
     || spawn_next_command_in_evd(evd) < 0
    ) {
        log_notice("done running event on '%s': %d", g_dump_dir_name, retval);
        append_to_textview(g_tv_event_log, "\n");

         
        strbuf_free(cmd_output);
        cmd_output = NULL;

         
        gtk_widget_hide(GTK_WIDGET(g_spinner_event_log));
        gtk_widget_hide(g_btn_stop);
         
        gtk_widget_set_sensitive(g_btn_close, true);
        gtk_widget_set_sensitive(g_btn_next, true);

        problem_data_reload_from_dump_dir();
        update_gui_state_from_problem_data(UPDATE_SELECTED_EVENT);

        if (retval != 0)
        {
            gtk_widget_show(GTK_WIDGET(g_img_process_fail));
             
            if (retval == 256)
                cancel_processing(g_lbl_event_log, _("Processing was interrupted because the problem is not reportable."), TERMINATE_NOFLAGS);
            else
            {
                 
                if (retval == EXIT_CANCEL_BY_USER || WTERMSIG(run_state->process_status) == SIGTERM)
                    cancel_processing(g_lbl_event_log,   NULL, TERMINATE_NOFLAGS);
                else
                {
                    cancel_processing(g_lbl_event_log, _("Processing failed."), TERMINATE_WITH_RERUN);
                    on_failed_event(evd->event_name);
                }
            }
        }
        else
        {
            gtk_label_set_text(g_lbl_event_log, is_processing_finished() ? _("Processing finished.")
                                                                         : _("Processing finished, please proceed to the next step."));
        }

        g_source_remove(g_event_source_id);
        g_event_source_id = 0;
        close(evd->fd);
        g_io_channel_unref(evd->channel);
        free_run_event_state(evd->run_state);
        strbuf_free(evd->event_log);
        free(evd->event_name);
        free(evd);

         
        kill(getppid(), SIGCHLD);

        if (is_processing_finished())
            hide_next_step_button();
        else if (retval == 0 && !g_verbose && !g_expert_mode)
            on_next_btn_cb(GTK_WIDGET(g_btn_next), NULL);

        return FALSE;  
    }

     

     
    xmove_fd(evd->run_state->command_out_fd, evd->fd);
    evd->run_state->command_out_fd = evd->fd;  
    ndelay_on(evd->fd);

     
    g_event_child_pid = evd->run_state->command_pid;

    return TRUE;  
}
