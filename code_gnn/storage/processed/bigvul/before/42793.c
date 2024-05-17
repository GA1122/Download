static char *ask_helper(const char *msg, void *args, bool password)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(g_wnd_assistant),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_OK_CANCEL,
            "%s", msg);
    char *tagged_msg = tag_url(msg, "\n");
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(dialog), tagged_msg);
    free(tagged_msg);

    GtkWidget *vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *textbox = gtk_entry_new();
     
    g_object_set(G_OBJECT(textbox), "editable", TRUE, NULL);
    g_signal_connect(textbox, "activate", G_CALLBACK(gtk_entry_emit_dialog_response_ok), dialog);

    if (password)
        gtk_entry_set_visibility(GTK_ENTRY(textbox), FALSE);

    gtk_box_pack_start(GTK_BOX(vbox), textbox, TRUE, TRUE, 0);
    gtk_widget_show(textbox);

    char *response = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        const char *text = gtk_entry_get_text(GTK_ENTRY(textbox));
        response = xstrdup(text);
    }

    gtk_widget_destroy(textbox);
    gtk_widget_destroy(dialog);

    const char *log_response = "";
    if (response)
        log_response = password ? "********" : response;

    log_request_response_communication(msg, log_response, (struct analyze_event_data *)args);
    return response ? response : xstrdup("");
}