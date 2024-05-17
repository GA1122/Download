static void notifyStatus(WebKitWebFrame* frame, WebKitLoadStatus loadStatus)
{
    frame->priv->loadStatus = loadStatus;
    g_object_notify(G_OBJECT(frame), "load-status");

    WebKitWebView* webView = getViewFromFrame(frame);
    if (frame == webkit_web_view_get_main_frame(webView)) {
        webView->priv->loadStatus = loadStatus;
        g_object_notify(G_OBJECT(webView), "load-status");
    }
}
