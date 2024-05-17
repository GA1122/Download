static gboolean webkit_web_view_scroll_event(GtkWidget* widget, GdkEventScroll* event)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

    Frame* frame = core(webView)->mainFrame();
    if (!frame->view())
        return FALSE;

    PlatformWheelEvent wheelEvent(event);
    return frame->eventHandler()->handleWheelEvent(wheelEvent);
}
