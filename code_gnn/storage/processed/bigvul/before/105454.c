gboolean webkit_web_view_get_view_source_mode (WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

    if (Frame* mainFrame = core(webView)->mainFrame())
        return mainFrame->inViewSourceMode();

    return FALSE;
}
