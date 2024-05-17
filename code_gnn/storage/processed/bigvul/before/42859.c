static int run_event_gtk_ask_yes_no(const char *msg, void *args)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(g_wnd_assistant),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO,
            "%s", msg);
    char *tagged_msg = tag_url(msg, "\n");
    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(dialog), tagged_msg);
    free(tagged_msg);

     
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_YES);
    const int ret = gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES;

    gtk_widget_destroy(dialog);

    log_request_response_communication(msg, ret ? "YES" : "NO", (struct analyze_event_data *)args);
    return ret;
}