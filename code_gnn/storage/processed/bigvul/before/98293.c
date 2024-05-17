Frame* FrameLoaderClient::dispatchCreatePage()
{
    WebKitWebView* webView = getViewFromFrame(m_frame);
    WebKitWebView* newWebView = 0;

    g_signal_emit_by_name(webView, "create-web-view", m_frame, &newWebView);

    if (!newWebView)
        return 0;

    WebKitWebViewPrivate* privateData = WEBKIT_WEB_VIEW_GET_PRIVATE(newWebView);
    return core(privateData->mainFrame);
}
