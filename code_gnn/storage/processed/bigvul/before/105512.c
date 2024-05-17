void webkit_web_view_set_maintains_back_forward_list(WebKitWebView* webView, gboolean flag)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    static_cast<BackForwardListImpl*>(core(webView)->backForwardList())->setEnabled(flag);
}
