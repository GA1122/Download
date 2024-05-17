static void webkit_web_view_grab_focus(GtkWidget* widget)
{

    if (gtk_widget_is_sensitive(widget)) {
        WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
        FocusController* focusController = core(webView)->focusController();

        focusController->setActive(true);

        if (focusController->focusedFrame())
            focusController->setFocused(true);
        else
            focusController->setFocusedFrame(core(webView)->mainFrame());
    }

    return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->grab_focus(widget);
}
