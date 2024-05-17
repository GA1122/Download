static int ask_replace_old_private_group_name(void)
{
    char *message = xasprintf(_("Private ticket is requested but the group name 'private' has been deprecated. "
                                "We kindly ask you to use 'fedora_contrib_private' group name. "
                                "Click Yes button or update the configuration manually. Or click No button, if you really want to use 'private' group.\n\n"
                                "If you are not sure what this dialogue means, please trust us and click Yes button.\n\n"
                                "Read more about the private bug reports at:\n"
                                "https://github.com/abrt/abrt/wiki/FAQ#creating-private-bugzilla-tickets\n"
                                "https://bugzilla.redhat.com/show_bug.cgi?id=1044653\n"));

    char *markup_message = xasprintf(_("Private ticket is requested but the group name <i>private</i> has been deprecated. "
                                "We kindly ask you to use <i>fedora_contrib_private</i> group name. "
                                "Click Yes button or update the configuration manually. Or click No button, if you really want to use <i>private</i> group.\n\n"
                                "If you are not sure what this dialogue means, please trust us and click Yes button.\n\n"
                                "Read more about the private bug reports at:\n"
                                "<a href=\"https://github.com/abrt/abrt/wiki/FAQ#creating-private-bugzilla-tickets\">"
                                "https://github.com/abrt/abrt/wiki/FAQ#creating-private-bugzilla-tickets</a>\n"
                                "<a href=\"https://bugzilla.redhat.com/show_bug.cgi?id=1044653\">https://bugzilla.redhat.com/show_bug.cgi?id=1044653</a>\n"));

    GtkWidget *old_private_group = gtk_message_dialog_new(GTK_WINDOW(g_wnd_assistant),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_WARNING,
        GTK_BUTTONS_YES_NO,
        message);

    gtk_window_set_transient_for(GTK_WINDOW(old_private_group), GTK_WINDOW(g_wnd_assistant));
    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(old_private_group),
                                    markup_message);
    free(message);
    free(markup_message);

     
    gtk_dialog_set_default_response(GTK_DIALOG(old_private_group), GTK_RESPONSE_YES);

    gint result = gtk_dialog_run(GTK_DIALOG(old_private_group));
    gtk_widget_destroy(old_private_group);

    return result == GTK_RESPONSE_YES;
}