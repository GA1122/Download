WebKitWebBackForwardList* webkit_web_view_get_back_forward_list(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
    if (!core(webView) || !static_cast<BackForwardListImpl*>(core(webView)->backForwardList())->enabled())
        return 0;
    return webView->priv->backForwardList.get();
}
