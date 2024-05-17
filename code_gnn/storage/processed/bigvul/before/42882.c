static void update_command_run_log(const char* message, struct analyze_event_data *evd)
{
    const bool it_is_a_dot = (message[0] == '.' && message[1] == '\0');

    if (!it_is_a_dot)
        gtk_label_set_text(g_lbl_event_log, message);

     
    const char *log_msg = it_is_a_dot ? message : xasprintf("%s\n", message);
    append_to_textview(g_tv_event_log, log_msg);
    save_to_event_log(evd, log_msg);

     
    if (log_msg != message)
        free((void *)log_msg);
}
