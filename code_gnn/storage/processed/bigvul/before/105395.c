static gboolean webkit_web_view_button_release_event(GtkWidget* widget, GdkEventButton* event)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

    Frame* focusedFrame = core(webView)->focusController()->focusedFrame();

    if (focusedFrame && focusedFrame->editor()->canEdit()) {
#ifdef MAEMO_CHANGES
        WebKitWebViewPrivate* priv = webView->priv;
        hildon_gtk_im_context_filter_event(priv->imContext.get(), (GdkEvent*)event);
#endif
    }

    Frame* mainFrame = core(webView)->mainFrame();
    if (mainFrame->view())
        mainFrame->eventHandler()->handleMouseReleaseEvent(PlatformMouseEvent(event));

     

    return FALSE;
}
