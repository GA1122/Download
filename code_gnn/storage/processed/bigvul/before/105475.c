void webkit_web_view_notify_ready(WebKitWebView* webView)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    gboolean isHandled = FALSE;
    g_signal_emit(webView, webkit_web_view_signals[WEB_VIEW_READY], 0, &isHandled);
}
