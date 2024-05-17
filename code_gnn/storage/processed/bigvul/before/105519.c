void webkit_web_view_set_view_source_mode (WebKitWebView* webView, gboolean mode)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    if (Frame* mainFrame = core(webView)->mainFrame())
        mainFrame->setInViewSourceMode(mode);
}
