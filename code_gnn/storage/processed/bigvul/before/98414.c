GtkPrintOperationResult webkit_web_frame_print_full(WebKitWebFrame* frame, GtkPrintOperation* operation, GtkPrintOperationAction action, GError** error)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), GTK_PRINT_OPERATION_RESULT_ERROR);
    g_return_val_if_fail(GTK_IS_PRINT_OPERATION(operation), GTK_PRINT_OPERATION_RESULT_ERROR);

    GtkWidget* topLevel = gtk_widget_get_toplevel(GTK_WIDGET(webkit_web_frame_get_web_view(frame)));
    if (!GTK_WIDGET_TOPLEVEL(topLevel))
        topLevel = NULL;

    Frame* coreFrame = core(frame);
    if (!coreFrame)
        return GTK_PRINT_OPERATION_RESULT_ERROR;

    PrintContext printContext(coreFrame);

    g_signal_connect(operation, "begin-print", G_CALLBACK(begin_print_callback), &printContext);
    g_signal_connect(operation, "draw-page", G_CALLBACK(draw_page_callback), &printContext);
    g_signal_connect(operation, "end-print", G_CALLBACK(end_print_callback), &printContext);

    return gtk_print_operation_run(operation, action, GTK_WINDOW(topLevel), error);
}
