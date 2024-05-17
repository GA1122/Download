static gboolean webkit_web_view_key_press_event(GtkWidget* widget, GdkEventKey* event)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

    Frame* frame = core(webView)->focusController()->focusedOrMainFrame();
    PlatformKeyboardEvent keyboardEvent(event);

    if (!frame->view())
        return FALSE;

    if (frame->eventHandler()->keyEvent(keyboardEvent))
        return TRUE;

     
    return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->key_press_event(widget, event);
}
