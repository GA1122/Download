static gboolean webkit_web_view_key_release_event(GtkWidget* widget, GdkEventKey* event)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

    WebKit::EditorClient* client = static_cast<WebKit::EditorClient*>(core(webView)->editorClient());
    if (gtk_im_context_filter_keypress(webView->priv->imContext.get(), event) && !client->hasPendingComposition())
        return TRUE;

    Frame* frame = core(webView)->focusController()->focusedOrMainFrame();
    if (!frame->view())
        return FALSE;

    PlatformKeyboardEvent keyboardEvent(event);
    if (frame->eventHandler()->keyEvent(keyboardEvent))
        return TRUE;

     
    return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->key_release_event(widget, event);
}
