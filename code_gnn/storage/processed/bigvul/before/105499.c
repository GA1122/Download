void webkit_web_view_redo(WebKitWebView* webView)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    if (webkit_web_view_can_redo(webView))
        g_signal_emit(webView, webkit_web_view_signals[REDO], 0);
}
