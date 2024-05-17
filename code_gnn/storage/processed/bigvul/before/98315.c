void FrameLoaderClient::dispatchDidReceiveIcon()
{
    WebKitWebView* webView = getViewFromFrame(m_frame);

    g_signal_emit_by_name(webView, "icon-loaded", m_frame);
}
