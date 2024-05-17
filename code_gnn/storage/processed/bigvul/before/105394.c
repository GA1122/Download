static gboolean webkit_web_view_button_press_event(GtkWidget* widget, GdkEventButton* event)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    WebKitWebViewPrivate* priv = webView->priv;

    gtk_widget_grab_focus(widget);

    if (!priv->clickCounter.shouldProcessButtonEvent(event))
        return TRUE;

    PlatformMouseEvent platformEvent(event);
    platformEvent.setClickCount(priv->clickCounter.clickCountForGdkButtonEvent(widget, event));

    if (event->button == 3)
        return webkit_web_view_forward_context_menu_event(webView, PlatformMouseEvent(event));

    Frame* frame = core(webView)->mainFrame();
    if (!frame->view())
        return FALSE;

    gboolean result = frame->eventHandler()->handleMousePressEvent(platformEvent);
    static_cast<WebKit::EditorClient*>(core(webView)->editorClient())->handleInputMethodMousePress();

#if PLATFORM(X11)
     
    if (event->button == 2) {
        bool primary = webView->priv->usePrimaryForPaste;
        webView->priv->usePrimaryForPaste = true;

        Editor* editor = webView->priv->corePage->focusController()->focusedOrMainFrame()->editor();
        result = result || editor->canPaste() || editor->canDHTMLPaste();
        editor->paste();

        webView->priv->usePrimaryForPaste = primary;
    }
#endif

    return result;
}
