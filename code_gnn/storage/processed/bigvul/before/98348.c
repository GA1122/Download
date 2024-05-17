void FrameLoaderClient::provisionalLoadStarted()
{
    WebKitWebView* webView = getViewFromFrame(m_frame);

    if (m_frame == webkit_web_view_get_main_frame(webView))
        webkit_web_view_clear_resources(webView);
}
