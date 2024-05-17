const char* webkit_web_view_get_custom_encoding(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);
    String overrideEncoding = core(webView)->mainFrame()->loader()->documentLoader()->overrideEncoding();
    if (overrideEncoding.isEmpty())
        return 0;
    webView->priv->customEncoding = overrideEncoding.utf8();
    return webView->priv->customEncoding.data();
}
