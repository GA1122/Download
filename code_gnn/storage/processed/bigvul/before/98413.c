void webkit_web_frame_print(WebKitWebFrame* frame)
{
    g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));

    WebKitWebFramePrivate* priv = frame->priv;
    GtkPrintOperation* operation = gtk_print_operation_new();
    GError* error = 0;

    webkit_web_frame_print_full(frame, operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, &error);
    g_object_unref(operation);

    if (error) {
        GtkWidget* window = gtk_widget_get_toplevel(GTK_WIDGET(priv->webView));
        GtkWidget* dialog = gtk_message_dialog_new(GTK_WIDGET_TOPLEVEL(window) ? GTK_WINDOW(window) : 0,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_CLOSE,
                                                   "%s", error->message);
        g_error_free(error);

        g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
        gtk_widget_show(dialog);
    }
}
