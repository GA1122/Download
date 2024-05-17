void FrameLoaderClient::postProgressFinishedNotification()
{
    WebKitWebView* webView = getViewFromFrame(m_frame);
    WebKitWebViewPrivate* privateData = WEBKIT_WEB_VIEW_GET_PRIVATE(webView);

    if (!privateData->disposing)
        g_signal_emit_by_name(webView, "load-finished", m_frame);
}
